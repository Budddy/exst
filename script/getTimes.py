import os
import threading
import subprocess
import traceback
from os import listdir
from os.path import isfile, join
import time

import signal

# maxTime - maximum runtime in Seconds per program Execution
maxTime = 2000

# programs to test (path to program / name)
programs = [("./claspfolio-2.2.0/binaries/clasp-3.1.1-x86_64-linux ", "default_clasp"),
            ("python ./claspfolio-2.2.0/src/claspfolio.py -I ", "default_claspfolio"),
            ("python ./claspfolio-2.2.0_patched/src/claspfolio.py -I ", "patched_claspfolio")]

# path to the folder with the ground test data files
testData = "./instances"

# number of runs per program/instance
runs = 2


class Command(object):
    """
    Enables to run subprocess commands in a different thread with TIMEOUT option.
    Based on:
    https://gist.github.com/kirpit/1306188
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

        kwargs['stdout'] = subprocess.PIPE
        kwargs['stderr'] = subprocess.PIPE

        start = time.time()
        thread = threading.Thread(target=target, kwargs=kwargs)
        thread.start()
        thread.join(timeout)
        end = time.time()
        if thread.is_alive():
            # kill thread if program takes too long to finish
            os.killpg(os.getpgid(self.process.pid), signal.SIGTERM)
            # wait for thread to be killed
            thread.join()
            # programm took too long to solve the instance
            runCompleted = 0
        else:
            # run completed in time
            runCompleted = 1
        return self.status, self.output, self.error, end - start, runCompleted


result = open(testData + "/result/_result_", 'a')
for a in listdir(testData):
    f = join(testData, a)
    if isfile(f):
        print "\ninstance: " + a
        for i in programs:
            print "  program: " + i[1]
            for count in range(1, runs):
                com = i[0] + os.path.dirname(f) + "/" + os.path.basename(f)
                c = Command(com)
                ret = c.run(timeout=maxTime)
                # if program takes too long, don't start more runs of the program for this test instance
                if ret[3] == 1:
                    print "    could not solve instance within time limit"
                    fi = open(testData + "/result/" + i[1] + "_failed_" + a, 'w')
                    fi.write(str(ret[1]))
                    fi.close()
                    result.write("    could not solve instance within time limit")
                    result.flush()
                    break
                else:
                    for s in ret[1].split("\n"):
                        if "CPU Time" in s:
                            print "    execution time: " + str(ret[3]) + "      " + s
                            fi = open(testData + "/result/" + i[1] + "_" + a + "_number_" + str(count), 'w')
                            fi.write(str(ret[1]))
                            fi.close()
                            result.write("    execution time: " + str(ret[3]) + "      " + s)
                            result.flush()
result.close()
