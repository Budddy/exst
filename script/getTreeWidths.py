import csv
import gzip
import json
import os
import string
import subprocess
import threading
import time
import traceback
import signal
from os.path import isfile, join

timeWidth = 180
outputFile = "./out.txt"

dirTimes = "./Times/"
dirStats = "./Stats/"
csvFile = "./Stats.csv"
csvFileTmp = "./Stats_tmp.csv"
tmpFile = "./tmp.txt"
dirCSVFinished = "./CSV_finished/"

# commands for Windows
"""
linuxSystem = False
widthCommand = "htd_main -s 1234 --output width --opt width < "
"""

# commands for Linux
linuxSystem = True
widthCommand = "./htd_main -s 1234 --output width --opt width < "

gen_r_width = True
gen_i_width = True
gen_d_width = True


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

        def target(**kwargs_):
            try:
                if linuxSystem:
                    self.process = subprocess.Popen(self.command, shell=True,
                                                    preexec_fn=os.setsid,
                                                    **kwargs_)
                else:
                    self.process = subprocess.Popen(self.command, shell=True,
                                                    **kwargs_)
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
        stop = False
        if thread.is_alive():
            if linuxSystem:
                os.killpg(os.getpgid(self.process.pid), signal.SIGTERM)
            else:
                subprocess.call(
                    ['taskkill', '/F', '/T', '/PID', str(self.process.pid)])
            thread.join()
            stop = True
        return self.status, self.output, self.error, end - start, stop


# generate Graph stats
def gen_graph_stats(g, calc_width=True):
    tmp = open(tmpFile, "w")
    node_count = 0
    edge_count = str(len(g) - 1)
    for line in g[1:]:
        for l in line.split(' '):
            node_count = node_count if node_count > int(l) else int(l)
    g[0] = 'p tw ' + str(node_count) + ' ' + edge_count
    tmp.write(string.join(g, "\n"))
    tmp.close()
    ret = [0, 0, 0, True]
    if calc_width:
        ret = Command(widthCommand + tmpFile).run(timeWidth)
    return [ret[1], int(edge_count), int(node_count), ret[4]]


# generate CSV
def get_csv(row):
    print ("inst: " + row["inst"])
    print ("  CSV...")
    file_name = row["inst"]

    with open(outputFile, "a") as out:
        out.write("  CSV..." + "\n")

    names_csv = ["inst", "time_total", "time_CPU", "result", "choices",
                 "conflicts", "backtracks", "backjumps", "restarts", "jumps",
                 "atoms", "rules", "bodies", "equivalences", "variables",
                 "eliminated", "frozen", "bodies", "nHornClauses",
                 "nDualHornClauses", "maxClauseSize", "maxPositiveClauseSize",
                 "maxNegativeClauseSize", "numVPosLitH", "numVPosLit",
                 "numVNegLitH", "numVNegLit", "maxPosRSizeC", "maxPosRSizeNC",
                 "numAtomOccC", "numAtomOccNC", "maxNumAtomOcc",
                 "maxNumPosOccA", "maxNumNegOccA", "dWidth", "dEdges",
                 "dNodes", "iWidth", "iEdges", "iNodes", "rEdges0", "rNodes0",
                 "rWidths0", "rEdges1", "rNodes1", "rWidths1", "rEdges2",
                 "rNodes2", "rWidths2", "rEdges3", "rNodes3", "rWidths3",
                 "rEdges4", "rNodes4", "rWidths4", "rEdges5", "rNodes5",
                 "rWidths5", "rEdges6", "rNodes6", "rWidths6", "rEdges7",
                 "rNodes7", "rWidths7", "rEdges8", "rNodes8", "rWidths8",
                 "rEdges9", "rNodes9", "rWidths9"]

    if not isfile(csvFileTmp):
        with open(csvFileTmp, "w") as statsFile:
            writer = csv.DictWriter(statsFile, fieldnames=names_csv,
                                    delimiter=',', lineterminator='\n')
            writer.writeheader()

    if isfile(join(dirStats, file_name)):
        with gzip.open(join(dirStats, file_name), "r") as myfile:
            data = myfile.read()
            d = json.loads(data)
            # get Dependency Graph Stats
            print ("      dependency Graph")
            with open(outputFile, "a") as out:
                out.write("      dependency Graph" + "\n")
            if row["dWidth"] == '':
                ret = gen_graph_stats(d['Stats']['Incidence Graph'])
                if not ret[3]:
                    print ("        success")
                    row["dWidth"] = int(ret[0])
                else:
                    print ("        failure")

            # get Incidence Graph Stats
            print ("      incidence Graph")
            with open(outputFile, "a") as out:
                out.write("      incidence Graph" + "\n")
            if row["dWidth"] == '':
                ret = gen_graph_stats(d['Stats']['Incidence Graph'])
                if not ret[3]:
                    print ("        success")
                    row["iWidth"] = int(ret[0])
                else:
                    print ("        failure")

            # get ReductGraph Stats
            print ("      reduct Graph")
            with open(outputFile, "a") as out:
                out.write("      reduct Graph" + "\n")
            for i in range(0, 10):
                print ("        " + str(i))
                with open(outputFile, "a") as out:
                    out.write("        " + str(i) + "\n")
                if len(d['Stats']['Reduct Graph']) > i:
                    if row["rWidths" + str(i)] == '':
                        ret = gen_graph_stats(d['Stats']['Reduct Graph'][i])
                        if not ret[3]:
                            print ("          success")
                            row["rWidths" + str(i)] = int(ret[0])
                        else:
                            print ("          failure")

    with open(csvFileTmp, "a") as statsFile:
        writer = csv.DictWriter(statsFile, fieldnames=names_csv, delimiter=',',
                                lineterminator='\n')
        writer.writerow(row)


names_csv = ["inst", "time_total", "time_CPU", "result", "choices",
             "conflicts", "backtracks", "backjumps", "restarts", "jumps",
             "atoms", "rules", "bodies", "equivalences", "variables",
             "eliminated", "frozen", "bodies", "nHornClauses",
             "nDualHornClauses", "maxClauseSize", "maxPositiveClauseSize",
             "maxNegativeClauseSize", "numVPosLitH", "numVPosLit",
             "numVNegLitH", "numVNegLit", "maxPosRSizeC", "maxPosRSizeNC",
             "numAtomOccC", "numAtomOccNC", "maxNumAtomOcc", "maxNumPosOccA",
             "maxNumNegOccA", "dWidth", "dEdges", "dNodes", "iWidth", "iEdges",
             "iNodes", "rEdges0", "rNodes0", "rWidths0", "rEdges1", "rNodes1",
             "rWidths1", "rEdges2", "rNodes2", "rWidths2", "rEdges3",
             "rNodes3", "rWidths3", "rEdges4", "rNodes4", "rWidths4",
             "rEdges5", "rNodes5", "rWidths5", "rEdges6", "rNodes6",
             "rWidths6", "rEdges7", "rNodes7", "rWidths7", "rEdges8",
             "rNodes8", "rWidths8", "rEdges9", "rNodes9", "rWidths9"]

linenum = 0
with open('Stats.csv', 'r') as f:
    reader = csv.DictReader(f, fieldnames=names_csv, delimiter=',',
                            lineterminator='\n')
    for row in reader:
        if reader.line_num > 1:
            linenum += 1
            print("line:" + str(linenum))
            get_csv(row)
