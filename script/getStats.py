import gzip
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
maxTime = 2000

# programs to test (path to program / name)
program = "./exst --printDgraph=1 --printIgraph=1 --printRgraph=1 -q 2 --stats=2 --time-limit=1900 --outf=2 -V "

# path to the folder with the ground test data files
testData = "./Instances"

# directory for the result
resultDir = "./Stats"

# directory for the result
failedDir = "./Stats/stats_failed"


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
        def target(**kwargs):
            try:
                self.process = subprocess.Popen(self.command, shell=True,
                                                preexec_fn=os.setsid, **kwargs)
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
            os.killpg(os.getpgid(self.process.pid), signal.SIGTERM)
            thread.join()
        return self.status, self.output, self.error, end - start


if not isfile(resultDir + "/_result_"):
    result = open(resultDir + "/_result_", 'w')
else:
    result = open(resultDir + "/_result_", 'a')

assignments = json.loads(open("./_assignments_", 'r').read())
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
        result.write("  clasp:\n")
        result.flush()
        # only run if file doesn't already exist
        if not (isfile(resultDir + "/" + a) or isfile(
                        failedDir + "/" + a) or isfile(
                            failedDir + "/" + a + '.gz') or isfile(
                            resultDir + "/" + a + '.gz')):
            com = program + os.path.dirname(f) + "/" + os.path.basename(
                f) + " --width-intervall=" + str(assignments[a] / 10)
            print ("  command: " + com)
            c = Command(com)
            ret = c.run(timeout=maxTime)
            # if program takes too long, don't start more runs of the program for this test instance
            print ("    execution time: " + str(ret[3]))
            result.write("    execution time: " + str(ret[3]) + "\n")
            result.flush()
            if ret[3] > 1800:
                fi = gzip.open(failedDir + "/" + a + ".gz", 'w')
            else:
                fi = gzip.open(resultDir + "/" + a + ".gz", 'w')
            fi.write(str(ret[1]))
            fi.flush()
            fi.close()
result.close()
