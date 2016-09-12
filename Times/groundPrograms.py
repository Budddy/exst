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
                self.process = subprocess.Popen(self.command, shell=True, preexec_fn=os.setsid, **kwargs)
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
for tdir in listdir(dirs):
    numElements += len(listdir(join(dirs, tdir)))

for d in listdir(dirs):
    for a in listdir(dirs + d):
        countElements += 1
        f = join(dirs + d, a)
        print ("\ninstance(" + str(countElements) + "/" + str(numElements) + "): " + a)
        if isfile(f) and not (isfile(resultdir + a) or isfile(failed + a)) and (a != "encoding.asp"):
            com = "./gringo.exe " + os.path.dirname(f) + "/" + os.path.basename(f) + " " + os.path.dirname(
                f) + "/" + "encoding.asp"
            c = Command(com)
            ret = c.run(timeout=maxTime)
            if (len(str(ret[1])) > 10000000):
                print (" - too big")
                fi = open(failed + a, 'w')
                fi.close()
            elif (not ret[4]):
                print (" - ground")
                fi = open(resultdir + a, 'w')
                fi.write(str(ret[1]))
                fi.close()
            else:
                print (" - fail")
                fi = open(failed + a, 'w')
                fi.close()
