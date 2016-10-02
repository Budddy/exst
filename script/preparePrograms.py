import csv
import gzip
import json
import os
import string
import subprocess
import threading
import time
import traceback
from os import listdir
from os.path import isfile, join, isdir

# maximal time for grounding
groundTime = 60
# maximal time to get assignment number
assignmentTime = 1800
# maximal time for clasp
timesTime = 600
# maximal time for statistics calculation
statsTime = 1800
# maximal time for width calculation
timeWidth = 300

# file to print the output to
outputFile = "./out.txt"

# max Sizes for ground File in MB
maxGroundSize = 3

# directories
# directory where the asp programs are located
dirPrograms = "./Programs/"
# directory where the ground programs are located
dirGround = "./Ground/"
# directory for oversize ground programs
dirGroundOversize = "./Ground_oversize/"
# direcotry for programs that took too long to ground
dirGroundTimeout = "./Ground_timeout/"
# directory for programs that had an error during grounding
dirGroundError = "./Ground_error/"
# directory to save the clasp solving times to
dirTimes = "./Times/"
# directory to save the programs that took too long for clasp to solve
dirTimesFailed = "./Times_failed/"
# directory for the assignment counts
dirAssignments = "./Assignments/"
# directory for the statistics
dirStats = "./Stats/"
# directory for the failed statistics generations
dirStatsFailed = "./Stats_failed/"
# directory to save the graphs to
dirGraphs = "./Graphs/"
# directory to save the files to that are already in the csv
dirCSVFinished = "./CSV_finished/"
# CSV file for the statistics
csvFile = "./Stats.csv"
# tmp file for width generation
tmpFile = "./tmp.txt"

# commands for Windows
unixSystem = False
# command used for grounding
groundCommand = "gringo "
# command used for clasp time generation
timesCommand = "clasp --stats=2 -q 2 --time-limit=" + str(
    timesTime + 10) + " --outf=2 -V "
# command used to calculate the number of assignments
assignmentCommand = "exst_assignments --stats=2 --time-limit=800 --outf=2 -V "
# command used to generate the width
widthCommand = "htd_main -s 1234 --output width --opt width < "
# command used to generate the statistics
statsCommand = "exst --printDgraph=1 --printIgraph=1 --printRgraph=1 -q 2 " \
               "--stats=2 --time-limit=" + str(
    statsTime + 10) + " --outf=2 -V "

# commands for Linux
"""
unixSystem = True
# command used for grounding
groundCommand = "./gringo "
# command used for clasp time generation
timesCommand = "./clasp --stats=2 -q 2 --time-limit=" \
               + str(timesTime + 10) + " --outf=2 -V "
# command used to calculate the number of assignments
assignmentCommand = "./exst_assignments --stats=2 --time-limit=800 " \
                    "--outf=2 -V "
# command used to generate the width
widthCommand = "./htd_main -s 1234 --output width --opt width < "
# command used to generate the statistics
statsCommand = "exst --printDgraph=1 --printIgraph=1 --printRgraph=1 -q 2 " \
               "--stats=2 --time-limit=" + str(statsTime+10) + " --outf=2 -V "
"""

# flag to generate the width of the reduct graphs
gen_r_width = True
# flag to generate the width of the incidence graph
gen_i_width = True
# flag to generate the width of the dependency graph
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
                if unixSystem:
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
            if unixSystem:
                self.process.kill()
            else:
                subprocess.call(
                    ['taskkill', '/F', '/T', '/PID', str(self.process.pid)])
            stop = True
        return self.status, self.output, self.error, end - start, stop


# ground the given Program
def ground_programs(file_name, prog_path):
    if not isfile(join(dirGround, file_name)) and not isfile(
            join(dirGroundError, file_name)) and not isfile(
            join(dirGroundTimeout, file_name)) and not isfile(
            join(dirGroundOversize, file_name)):
        print("  ground...")
        with open(outputFile, "a") as out:
            out.write("  ground..." + "\n")
        com = "gringo.exe " + join(prog_path, file_name) + " " + join(
            prog_path, "encoding.asp")
        c = Command(com)
        ret = c.run(timeout=groundTime)
        if len(str(ret[1])) > maxGroundSize * 1000000:
            print ("    too big")
            with open(outputFile, "a") as out:
                out.write("  too big" + "\n")
            fi = open(dirGroundOversize + file_name, 'w')
            fi.close()
        elif ret[4]:
            print ("    timeout")
            with open(outputFile, "a") as out:
                out.write("  timeout" + "\n")
            fi = open(dirGroundTimeout + file_name, 'w')
            fi.write("timeout")
            fi.close()
        elif len(str(ret[1])) == 0:
            print ("    error")
            with open(outputFile, "a") as out:
                out.write("  error" + "\n")
            fi = open(dirGroundError + file_name, 'w')
            fi.write(str(ret[2]))
            fi.close()
        elif not ret[4]:
            print ("    ground")
            with open(outputFile, "a") as out:
                out.write("  ground" + "\n")
            fi = open(dirGround + file_name, 'w')
            fi.write(str(ret[1]))
            fi.close()


# calculate Clasp Times
def get_times(file_name):
    if not isfile(join(dirTimes, file_name)) and isfile(
            join(dirGround, file_name)) and not isfile(
            join(dirTimesFailed, file_name)):
        print ("  times...")
        with open(outputFile, "a") as out:
            out.write("  times..." + "\n")
        # only run if file doesn't already exist
        com = timesCommand + join(dirGround + file_name)
        c = Command(com)
        ret = c.run(timeout=timesTime + 60)
        # if program takes too long, don't start more runs of the program
        # for this test instance
        j = json.loads(ret[1])
        s = j['Time']['Total']
        print ("    Time:" + str(s))
        with open(outputFile, "a") as out:
            out.write("    Time:" + str(s) + "\n")
        if s <= timesTime:
            fi = open(join(dirTimes, file_name), 'w')
        else:
            print("      failed")
            with open(outputFile, "a") as out:
                out.write("      failed" + "\n")
            fi = open(join(dirTimesFailed, file_name), 'w')
        fi.write(str(ret[1]))
        fi.flush()
        fi.close()


# calculate Assignments
def get_assignments(file_name):
    if not isfile(join(dirAssignments, file_name)) and isfile(
            join(dirTimes, file_name)):
        print ("  assignments...")
        with open(outputFile, "a") as out:
            out.write("  assignments..." + "\n")
        # only run if file doesn't already exist
        com = assignmentCommand
        c = Command(com)
        ret = c.run(timeout=assignmentTime + 60)
        j = json.loads(ret[1])
        assign = j['Stats']['numAssignments']
        assignments = open(join(dirAssignments, file_name), 'w')
        assignments.write(str(assign))
        assignments.flush()
        assignments.close()


# print Graphs into Graph dir, calc Stats
def get_stats(file_name):
    if isfile(join(dirAssignments, file_name + ".gz")) and isfile(
            join(dirTimes, file_name)) and not isfile(
            join(dirStats, file_name)) and not isfile(
            join(dirStatsFailed, file_name)):
        print("  stats...")
        with open(outputFile, "a") as out:
            out.write("  stats..." + "\n")
        # only run if file doesn't already exist
        assignments = int(open(join(dirAssignments, file_name), 'r').read())
        os.makedirs(join(dirGraphs, file_name))
        com = statsCommand + join(dirStats,
                                  file_name) + " --width-intervall=" + str(
            assignments / 10)
        c = Command(com)
        ret = c.run(timeout=statsTime + 60)
        # if program takes too long, don't start more runs of the program
        # for this test instance
        print ("    execution time: " + str(ret[3]))
        with open(outputFile, "a") as out:
            out.write("    execution time: " + str(ret[3]) + "\n")
        if ret[3] > statsTime:
            fi = gzip.open(join(dirStatsFailed, file_name + ".gz"), 'w')
        else:
            fi = gzip.open(join(dirStats, file_name + ".gz"), 'w')
        fi.write(str(ret[1]))
        fi.flush()
        fi.close()


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
    return [ret[1], int(edge_count), int(node_count), not ret[4]]


# generate CSV
def get_csv(file_name):
    if isfile(join(dirCSVFinished, file_name)):
        return

    print ("  CSV...")
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

    if not isfile(csvFile):
        with open(csvFile, "w") as statsFile:
            writer = csv.DictWriter(statsFile, fieldnames=names_csv,
                                    delimiter=',', lineterminator='\n')
            writer.writeheader()

    stats = {"inst": file_name}
    if isfile(join(dirTimes, file_name[:-3])):
        print("    clasp")
        with open(outputFile, "a") as out:
            out.write("    clasp" + "\n")
        with open(join(dirTimes, file_name[:-3]), "r") as myfile:
            data = myfile.read()
            d = json.loads(data)
            if 'Time' in d:
                stats["time_total"] = d['Time']["Total"]
                stats["time_CPU"] = d['Time']["CPU"]

            if 'Result' in d:
                stats["result"] = d['Result']

    if isfile(join(dirStats, file_name)):
        print ("    exst")
        with open(outputFile, "a") as out:
            out.write("    exst" + "\n")
        with gzip.open(join(dirStats, file_name), "r") as myfile:
            data = myfile.read()
            d = json.loads(data)

            if 'Stats' in d:
                if 'Core' in d['Stats']:
                    stats["choices"] = d['Stats']['Core']['Choices']
                    stats["conflicts"] = d['Stats']['Core']['Conflicts']
                    stats["backtracks"] = d['Stats']['Core']['Backtracks']
                    stats["backjumps"] = d['Stats']['Core']['Backjumps']
                    stats["restarts"] = d['Stats']['Core']['Restarts']

                if 'Jumps' in d['Stats']:
                    stats["jumps"] = d['Stats']['Jumps']['Sum']

                if 'LP' in d['Stats']:
                    stats["atoms"] = d['Stats']["LP"]["Atoms"]
                    stats["rules"] = d['Stats']["LP"]["Rules"]["Sum"]
                    stats["bodies"] = d['Stats']["LP"]["Bodies"]
                    stats["equivalences"] = d['Stats']["LP"]["Equivalences"][
                        "Sum"]

                if 'Problem' in d['Stats']:
                    stats["variables"] = d['Stats']['Problem']["Variables"]
                    stats["eliminated"] = d['Stats']['Problem']["Eliminated"]
                    stats["frozen"] = d['Stats']['Problem']["Frozen"]
                    stats["bodies"] = d['Stats']['Problem']["Constraints"][
                        "Sum"]

            stats["nHornClauses"] = d['Stats']['Extended Stats'][4][1]
            stats["nDualHornClauses"] = d['Stats']['Extended Stats'][5][1]
            stats["maxClauseSize"] = d['Stats']['Extended Stats'][6][1]
            stats["maxPositiveClauseSize"] = d['Stats']['Extended Stats'][7][1]
            stats["maxNegativeClauseSize"] = d['Stats']['Extended Stats'][8][1]
            stats["numVPosLitH"] = d['Stats']['Extended Stats'][9][1]
            stats["numVPosLit"] = d['Stats']['Extended Stats'][10][1]
            stats["numVNegLitH"] = d['Stats']['Extended Stats'][11][1]
            stats["numVNegLit"] = d['Stats']['Extended Stats'][12][1]
            stats["maxPosRSizeC"] = d['Stats']['Extended Stats'][13][1]
            stats["maxPosRSizeNC"] = d['Stats']['Extended Stats'][14][1]
            stats["numAtomOccC"] = d['Stats']['Extended Stats'][15][1]
            stats["numAtomOccNC"] = d['Stats']['Extended Stats'][16][1]
            stats["maxNumAtomOcc"] = d['Stats']['Extended Stats'][17][1]
            stats["maxNumPosOccA"] = d['Stats']['Extended Stats'][18][1]
            stats["maxNumNegOccA"] = d['Stats']['Extended Stats'][19][1]

            # get Dependency Graph Stats
            print ("      dependency Graph")
            with open(outputFile, "a") as out:
                out.write("      dependency Graph" + "\n")
            if gen_d_width:
                ret = gen_graph_stats(d['Stats']['Incidence Graph'])
                if not ret[3]:
                    stats["dWidth"] = ret[0]
            else:
                ret = gen_graph_stats(d['Stats']['Dependency Graph'], False)
            stats["dEdges"] = ret[1]
            stats["dNodes"] = ret[2]

            # get Incidence Graph Stats
            print ("      incidence Graph")
            with open(outputFile, "a") as out:
                out.write("      incidence Graph" + "\n")
            if gen_i_width:
                ret = gen_graph_stats(d['Stats']['Incidence Graph'])
                if not ret[3]:
                    stats["iWidth"] = ret[0]
            else:
                ret = gen_graph_stats(d['Stats']['Incidence Graph'], False)
            stats["iEdges"] = ret[1]
            stats["iNodes"] = ret[2]

            # get ReductGraph Stats
            print ("      reduct Graph")
            with open(outputFile, "a") as out:
                out.write("      reduct Graph" + "\n")
            for i in range(0, 10):
                print ("        " + str(i))
                with open(outputFile, "a") as out:
                    out.write("        " + str(i) + "\n")
                if len(d['Stats']['Reduct Graph']) > i:
                    if gen_r_width:
                        ret = gen_graph_stats(d['Stats']['Reduct Graph'][i])
                        if not ret[3]:
                            stats["rWidths" + str(i)] = ret[0]
                    else:
                        ret = gen_graph_stats(d['Stats']['Reduct Graph'][i],
                                              False)
                    stats["rEdges" + str(i)] = ret[1]
                    stats["rNodes" + str(i)] = ret[2]

    with open(csvFile, "a") as statsFile:
        writer = csv.DictWriter(statsFile, fieldnames=names_csv, delimiter=',',
                                lineterminator='\n')
        writer.writerow(stats)
        open(join(dirCSVFinished + file_name), 'w').close()


def create_missing_dirs():
    """
    create missing dirs
    """
    if not isfile(outputFile):
        open(outputFile, "w").close()
    if not isdir(dirPrograms):
        os.makedirs(dirPrograms)
    if not isdir(dirGround):
        os.makedirs(dirGround)
    if not isdir(dirGroundOversize):
        os.makedirs(dirGroundOversize)
    if not isdir(dirGroundTimeout):
        os.makedirs(dirGroundTimeout)
    if not isdir(dirGroundError):
        os.makedirs(dirGroundError)
    if not isdir(dirTimes):
        os.makedirs(dirTimes)
    if not isdir(dirTimesFailed):
        os.makedirs(dirTimesFailed)
    if not isdir(dirAssignments):
        os.makedirs(dirAssignments)
    if not isdir(dirStatsFailed):
        os.makedirs(dirStatsFailed)
    if not isdir(dirStats):
        os.makedirs(dirStats)
    if not isdir(dirGraphs):
        os.makedirs(dirGraphs)
    if not isdir(dirCSVFinished):
        os.makedirs(dirCSVFinished)


numElements = 0
countElements = 0
create_missing_dirs()
if len(listdir(dirPrograms)) > 0:
    for pr in listdir(dirPrograms):
        for tdir in listdir(join(dirPrograms, pr)):
            numElements += len(listdir(join(join(dirPrograms, pr), tdir)))
    for year in listdir(dirPrograms):
        for problem in listdir(join(dirPrograms, year)):
            p_dir = join(dirPrograms, year, problem)
            for inst in listdir(p_dir):
                countElements += 1
                print("instance(" + str(countElements) + "/" + str(
                    numElements) + "): " + inst)
                with open(outputFile, "a") as printFile:
                    printFile.write(
                        "instance(" + str(countElements) + "/" + str(
                            numElements) + "): " + inst + "\n")
                try:
                    if isfile(join(p_dir, inst)):
                        ground_programs(inst, p_dir)
                    get_times(inst)
                    get_assignments(inst)
                    get_stats(inst)
                    get_csv(inst)
                except:
                    print("__--error--__ " + inst)
                    with open(outputFile, "a") as printFile:
                        printFile.write("__--error--__ " + inst + "\n")
elif len(listdir(dirGround)) > 0:
    numElements = len(listdir(dirGround))
    for inst in listdir(dirGround):
        countElements += 1
        print("instance(" + str(countElements) + "/" + str(
            numElements) + "): " + inst)
        with open(outputFile, "a") as printFile:
            printFile.write("instance(" + str(countElements) + "/" + str(
                numElements) + "): " + inst + "\n")
        if isfile(join(dirGround, inst)):
            try:
                get_times(inst)
                get_assignments(inst)
                get_stats(inst)
                get_csv(inst)
            except:
                print("__--error--__ " + inst)
                with open(outputFile, "a") as printFile:
                    printFile.write("__--error--__ " + inst + "\n")
elif len(listdir(dirStats)) > 0 or len(listdir(dirTimes)) > 0:
    numElements = len(listdir(dirStats))
    for inst in listdir(dirStats):
        countElements += 1
        print("instance(" + str(countElements) + "/" + str(
            numElements) + "): " + inst)
        with open(outputFile, "a") as printFile:
            printFile.write("instance(" + str(countElements) + "/" + str(
                numElements) + "): " + inst + "\n")
        try:
            get_csv(inst)
        except:
            print("__--error--__ " + inst)
            with open(outputFile, "a") as printFile:
                printFile.write("__--error--__ " + inst + "\n")
else:
    print("no instances")
    with open(outputFile, "a") as printFile:
        printFile.write("no instances\n")
