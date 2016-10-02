import os
import signal
import subprocess
import threading
import time
import traceback
from os import listdir
from os.path import isfile, join

# maxTime in Seconds to run the test program
maxTime = 60

# path to the folder with the test data
dirs = "./Programs/"

# path for the ground programs
resultdir = "./Ground/All/"
errordir = "./Ground/Error/"
failed = "./Ground/Failed/"


class Command(object):
    """
    Enables to run subprocess commands in a different thread with TIMEOUT option.
    Based on jcollado's solution:
    http://stackoverflow.com/questions/1191374/subprocess-with-timeout/4825933#4825933
    """
    command = None
    process = None
    status = None
    output, error = '', ''

    def __init__(self, command):
        self.command = command

    def run(self, timeout=None, **kwargs):
        """ Run a command then return: (status, output, error). """

        def target(**kwargs):
            try:
                self.process = subprocess.Popen(self.command, shell=True,
                                                preexec_fn=os.setsid, **kwargs)
                # self.process = subprocess.Popen(self.command, shell=True, **kwargs)
                self.output, self.error = self.process.communicate()
                self.status = self.process.returncode
            except:
                self.error = traceback.format_exc()
                self.status = -1

        # default stdout and stderr
        if 'stdout' not in kwargs:
            kwargs['stdout'] = subprocess.PIPE
        if 'stderr' not in kwargs:
            kwargs['stderr'] = subprocess.PIPE
        # thread

        start = time.time()
        thread = threading.Thread(target=target, kwargs=kwargs)
        thread.start()
        thread.join(timeout)
        end = time.time()
        stop = 0
        if thread.is_alive():
            os.killpg(os.getpgid(self.process.pid), signal.SIGTERM)
            thread.join()
            stop = 1
        return self.status, self.output, self.error, end - start, stop


numElements = 0
countElements = 0
for pr in listdir(dirs):
    for tdir in listdir(join(dirs, pr)):
        numElements += len(listdir(join(join(dirs, pr), tdir)))

for year in listdir(dirs):
    for problem in listdir(join(dirs, year)):
        pdir = join(join(dirs, year), problem)
        for inst in listdir(pdir):
            ipath = join(pdir, inst)
            countElements += 1
            print ("\ninstance(" + str(countElements) + "/" + str(
                numElements) + "): " + inst)
            if isfile(ipath) and not (isfile(join(resultdir, inst)) or isfile(
                    join(failed, inst))) and (inst != "encoding.asp"):
                com = "./gringo " + os.path.dirname(
                    ipath) + "/" + os.path.basename(
                    ipath) + " " + os.path.dirname(
                    ipath) + "/" + "encoding.asp"
                c = Command(com)
                ret = c.run(timeout=maxTime)
                if len(str(ret[1])) > 10000000:
                    print (" - too big")
                    fi = open(failed + inst, 'w')
                    fi.close()
                elif len(str(ret[1])) == 0:
                    print (" - fail")
                    fi = open(errordir + inst, 'w')
                    fi.write(str(ret[2]))
                    fi.close()
                elif not ret[4]:
                    print (" - ground")
                    fi = open(resultdir + inst, 'w')
                    fi.write(str(ret[1]))
                    fi.close()
                else:
                    print (" - error")
                    fi = open(errordir + inst, 'w')
                    fi.write(str(ret[2]))
                    fi.close()
