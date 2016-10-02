import os
import signal
import subprocess
import threading
import time
import traceback
from os import listdir
from os.path import isfile, join
import json

# maxTime - maximum runtime in Seconds per program Execution
maxTime = 700

# programs to test (path to program / name)
programs = [("clasp.exe --stats=2 --time-limit=620 --outf=2 -V ", "clasp")]

# path to the folder with the ground test data files
testData = "./Ground/Times"

# directory for the result
resultDir = "./Result"

splitDir = "./Ground/Split"

# separate result into multiple folders
separate = 1

# number of runs per program/instance
runs = 1


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
                self.process = subprocess.Popen(self.command, shell=True,
                                                **kwargs)
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


if not isfile(resultDir + "/_result_"):
    result = open(resultDir + "/_result_", 'w')
else:
    result = open(resultDir + "/_result_", 'a')

numElements = len(listdir(testData))
countElements = 0

for a in listdir(testData):
    countElements += 1
    f = join(testData, a)
    if isfile(f):
        print ("\ninstance(" + str(countElements) + "/" + str(
            numElements) + "): " + a)
        result.write("\ninstance(" + str(countElements) + "/" + str(
            numElements) + "): " + a + "\n")
        result.flush()
        for i in programs:
            print ("  program: " + i[1])
            result.write("  program: " + i[1] + "\n")
            result.flush()
            for count in range(0, runs):
                # only run if file doesn't already exist
                if not (isfile(resultDir + "/result_1s/" + a) or isfile(
                                resultDir + "/result_10s/" + a) or isfile(
                            resultDir + "/result_100s/" + a) or isfile(
                            resultDir + "/result_1000s/" + a) or isfile(
                            resultDir + "/result_else/" + a) or isfile(
                            resultDir + "/result_10000s/" + a) or isfile(
                            resultDir + "/result_assignments/" + a) or isfile(
                            splitDir + "/1s/" + a) or isfile(
                            splitDir + "/10s/" + a) or isfile(
                            splitDir + "/100s/" + a) or isfile(
                            splitDir + "/1000s/" + a) or isfile(
                            splitDir + "/10000s/" + a)):
                    com = i[0] + os.path.dirname(f) + "/" + os.path.basename(f)
                    c = Command(com)
                    ret = c.run(timeout=maxTime)
                    # if program takes too long, don't start more runs of the program for this test instance
                    if ret[4] == 0:
                        print (
                            "    could not solve instance within time limit")
                        fi = open(resultDir + i[1] + "_failed_" + a, 'w')
                        fi.write(str(ret[1]))
                        fi.close()
                        result.write(
                            "    could not solve instance within time limit" + "\n")
                        result.flush()
                        break
                    else:
                        j = json.loads(ret[1])
                        s = j['Time']['CPU']
                        print ("    execution time: " + str(
                            ret[3]) + " CPU Time:" + str(s))
                        result.write("    execution time: " + str(
                            ret[3]) + " CPU Time:" + str(s) + "\n")
                        result.flush()
                        if s <= 1:
                            fi = open(resultDir + "/result_1s/" + a, 'w')
                        elif s <= 10:
                            fi = open(resultDir + "/result_10s/" + a, 'w')
                        elif s <= 100:
                            fi = open(resultDir + "/result_100s/" + a, 'w')
                        elif s <= 600:
                            fi = open(resultDir + "/result_1000s/" + a, 'w')
                        else:
                            fi = open(resultDir + "/result_else/" + a, 'w')
                        fi.write(str(ret[1]))
                        fi.flush()
                        fi.close()
result.close()
