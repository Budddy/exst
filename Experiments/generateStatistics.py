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
from os import listdir
from os.path import isfile, join, isdir

# ground Programs
groundPrograms = True
# get Clasp statistics
getClaspStatistics = True
# get extended statistics
getExtendedStatistics = True
# get the tree width
getTreeWidth = False
# generate the Statistic CSV
generateCSV = True

# maximal time for grounding
groundTime = 60
# maximal runtime time for clasp
timesTime = 600
# maximal time for statistics calculation
statsTime = 1800
# maximal time for width calculation
timeWidth = 1800

# file to print the output to
outputFile = "./out.txt"

# max Sizes for ground File in MB
maxGroundSize = 3

# directories
# directory where the asp programs are located
dirPrograms = "./Programs/"
# directory where the ground programs are located
dirGround = "./Ground/"
# directory to save the graphs to
dirGraphs = "./Graphs/"
# directory to save the clasp parameters
dirTimes = "./Times/"
# directory to save the extended paramters
dirExtendedStats = "./ExParamters"

# logfile for oversize ground programs
logGroundOversize = "./Ground_Oversize.log"
# logfile for programs that took too long to ground
logGroundTimeout = "./Ground_Timeout.log"
# logfile for programs that had an error during grounding
logGroundError = "./Ground_Error.log"
# logfile to save the programs that took too long for clasp to solve
logTimesFailed = "./Times_Failed.log"
# logfile for the failed parameter generations
logStatsFailed = "./Extended_Stats_Failed.log"
# CSV file for the parameters
csvFile = "./Parameters.csv"
# tmp file for width generation
tmpFile = "./tmp.txt"

# flag to generate the width of the reduct graphs
gen_r_width = False
# flag to generate the width of the incidence graph
gen_i_width = False
# flag to generate the width of the dependency graph
gen_d_width = False
# flag to generate the reduct
gen_reduct = False

# command used for grounding
groundCommand = "./gringo "
# command used for clasp time generation
timesCommand = "./clasp --stats=2 -q 2 --time-limit=" + str(timesTime + 10) + " --outf=2 -V "
# command used to generate the width
widthCommand = "./htd_main -s 1234 --output width --opt width < "
# command used to generate the extended parameters
statsCommand = "./exst -q 2 --stats=2 --time-limit=" + str(
    statsTime + 10) + " --printDgraph=1 " if gen_r_width else "" + " --printIgraph=1 " if gen_d_width else "" \
    + " --printRgraph=1 " if gen_i_width else "" + " --outf=2 -V "

names_csv = ["problem", "inst", "CPU_time", "numChoices", "numConflicts", "backtracks", "backjumps", "numRestarts", "jumps",
             "numAtoms", "numRules", "numBodies", "numEquivalences", "numVariables", "eliminated", "frozen", "numConstraints",
             "numNonHornClauses", "numNonDualHornClauses", "maxClauseSize", "maxPosClauseSize", "maxNegClauseSize",
             "numVPosLitH", "numVPosLit", "numVNegLitH", "numVNegLit", "maxPosRSizeC", "maxPosRSizeNC", "numAtomOccC",
             "numAtomOccNC", "maxNumAtomOcc", "maxNumPosOccA", "maxNumNegOccA", "dGraphWidth", "numDGraphEdges",
             "numDGraphNodes", "iGraphWidth",
             "numIGraphEdges", "numIGraphNodes", "numRGraphEdges0", "numRGraphNodes0", "rGraphWidth0",
             "numRGraphEdges1", "numRGraphNodes1", "rGraphWidth1", "numRGraphEdges2",
             "numRGraphNodes2", "rGraphWidth2", "numRGraphEdges3", "numRGraphNodes3", "rGraphWidth3", "numRGraphEdges4",
             "numRGraphNodes4", "rGraphWidth4", "numRGraphEdges5",
             "numRGraphNodes5", "rGraphWidth5", "numRGraphEdges6", "numRGraphNodes6", "rGraphWidth6", "numRGraphEdges7",
             "numRGraphNodes7", "rGraphWidth7", "numRGraphEdges8",
             "numRGraphNodes8", "rGraphWidth8", "numRGraphEdges9", "numRGraphNodes9", "rGraphWidth9", "problem_width",
             "maxSizeNCon",
             "maxSizeHeadNegBodyRule", "maxSizeRuleHead", "maxSizePosBodyNCon", "maxSizeNegBodyRule",
             "maxSizePosBodyCon", "maxSizeNegBodyCon", "numAtomsHead", "numAtomsPosBody", "numAtomsNegBody",
             "maxNumVarOcc", "maxNumVarOccHeadNegBody", "maxSizeAnswerSet"]


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
                self.process = subprocess.Popen(self.command, shell=True, preexec_fn=os.setsid, **kwargs_)
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
            os.killpg(os.getpgid(self.process.pid), signal.SIGTERM)
            thread.join()
            stop = True
        return self.status, self.output, self.error, end - start, stop


def print_message(msg):
    print(msg)
    with open(outputFile, "a") as out:
        out.write(msg + "\n")


def print_error(msg):
    print(msg)
    with open(outputFile, "a") as out:
        out.write(msg + "\n")


# ground the given Program
def ground_programs(file_name, prog_path):
    if not isfile(join(dirGround, file_name)):
        com = groundCommand + join(prog_path, file_name) + " " + join(prog_path, "encoding.asp")
        c = Command(com)
        ret = c.run(timeout=groundTime)
        if len(str(ret[1])) > maxGroundSize * 1000000:
            print_error("Oversize - instance: " + file_name)
            with open(logGroundOversize, "a") as out:
                out.write(file_name + "\n")
        elif ret[4]:
            print_error("Timeout - instance: " + file_name)
            with open(logGroundTimeout, "a") as out:
                out.write(file_name + "\n")
        elif len(str(ret[1])) == 0:
            print_error("Error - instance: " + file_name + " - " + ret[2])
            with open(logGroundError, "a") as out:
                out.write(file_name + " " + (str(ret[2])) + "\n")
        elif not ret[4]:
            print_message("Ground - instance: " + file_name)
            with open(dirGround + file_name, "w") as out:
                out.write(str(ret[1]))


# calculate Clasp Statistics
def get_times(file_name):
    if isfile(join(dirGround, file_name)) and not isfile(join(dirTimes, file_name)):
        print_message("Generate Clasp Statistics - instance: " + file_name)
        com = timesCommand + join(dirGround + file_name)
        c = Command(com)
        ret = c.run(timeout=timesTime + 60)
        j = json.loads(ret[1])
        s = j['Time']['Total']
        if s <= timesTime:
            with open(join(dirTimes, file_name), "w") as out:
                out.write(str(ret[1]))
        else:
            print_error("Timeout")
            with open(logTimesFailed, "a") as out:
                out.write(file_name + "\n")


# print Graphs into Graph dir, calc Stats
def get_stats(file_name):
    if isfile(join(dirTimes, file_name)) and not isfile(join(dirExtendedStats, file_name + ".gz")) and isfile(
            join(dirGround, file_name)):
        print_message("Generate Extended Statistics - instance: " + file_name)
        # only run if file doesn't already exist
        assignments = json.loads(open(join(dirTimes, file_name), 'r').read())['Stats']['Core']['Choices']
        com = statsCommand + join(dirGround, file_name) + " --width-intervall=" + (
            str(assignments / 11) if gen_reduct else "-1")
        c = Command(com)
        ret = c.run(timeout=statsTime + 60)
        if ret[3] > statsTime:
            print_error("Timeout")
            with open(logStatsFailed, "a") as out:
                out.write(file_name + "\n")
        else:
            fi = gzip.open(join(dirExtendedStats, file_name + ".gz"), 'w')
            fi.write(str(ret[1]))
            fi.close()


# generate Graph stats
def gen_graph_stats(g, calc_width=True):
    tmp = open(tmpFile, "w")
    node_count = g[0].split(' ')[2]
    edge_count = g[0].split(' ')[3]
    g[0] = 'p tw ' + str(node_count) + ' ' + edge_count
    tmp.write(string.join(g, "\n"))
    tmp.close()
    if calc_width:
        ret = Command(widthCommand + tmpFile).run(timeWidth)
        return [ret[1], int(edge_count), int(node_count), int(ret[4])]
    else:
        return [0, int(edge_count), int(node_count), True]


# generate CSV
def get_csv(file_name):
    print_message("  CSV...")
    if not isfile(csvFile):
        with open(csvFile, "w") as statsFile:
            writer = csv.DictWriter(statsFile, fieldnames=names_csv, delimiter=',', lineterminator='\n')
            writer.writeheader()
    stats = {"inst": file_name[:-3]}
    stats['problem'] = matches[stats['inst']]
    if isfile(join(dirTimes, file_name[:-3])):
        print_message("    clasp")
        with open(join(dirTimes, file_name[:-3]), "r") as myfile:
            data = myfile.read()
            d = json.loads(data)
            stats["CPU_time"] = d['Time']["CPU"]
    if isfile(join(dirExtendedStats, file_name)):
        print_message("    exst")
        with gzip.open(join(dirExtendedStats, file_name), "r") as myfile:
            data = myfile.read()
            d = json.loads(data)
            if 'Stats' in d:
                if 'Core' in d['Stats']:
                    stats["numChoices"] = d['Stats']['Core']['Choices']
                    stats["numConflicts"] = d['Stats']['Core']['Conflicts']
                    stats["backtracks"] = d['Stats']['Core']['Backtracks']
                    stats["backjumps"] = d['Stats']['Core']['Backjumps']
                    stats["numRestarts"] = d['Stats']['Core']['Restarts']
                if 'Jumps' in d['Stats']:
                    stats["jumps"] = d['Stats']['Jumps']['Sum']
                if 'LP' in d['Stats']:
                    stats["numAtoms"] = d['Stats']["LP"]["Atoms"]
                    stats["numRules"] = d['Stats']["LP"]["Rules"]["Sum"]
                    stats["numBodies"] = d['Stats']["LP"]["Bodies"]
                    stats["numEquivalences"] = d['Stats']["LP"]["Equivalences"]["Sum"]
                if 'Problem' in d['Stats']:
                    stats["numVariables"] = d['Stats']['Problem']["Variables"]
                    stats["eliminated"] = d['Stats']['Problem']["Eliminated"]
                    stats["frozen"] = d['Stats']['Problem']["Frozen"]
                    stats["numConstraints"] = d['Stats']['Problem']["Constraints"]["Sum"]
                if 'Extended Stats' in d['Stats']:
                    ex = d['Stats']['Extended Stats']
                    stats["numDGraphNodes"] = ex[0][1]
                    stats["numDGraphEdges"] = ex[1][1]
                    stats["numIGraphNodes"] = ex[2][1]
                    stats["numIGraphEdges"] = ex[3][1]
                    stats["numNonHornClauses"] = ex[4][1]
                    stats["numNonDualHornClauses"] = ex[5][1]
                    stats["maxClauseSize"] = ex[6][1]
                    stats["maxPosClauseSize"] = ex[7][1]
                    stats["maxNegClauseSize"] = ex[8][1]
                    stats["numVPosLitH"] = ex[9][1]
                    stats["numVPosLit"] = ex[10][1]
                    stats["numVNegLitH"] = ex[11][1]
                    stats["numVNegLit"] = ex[12][1]
                    stats["maxPosRSizeC"] = ex[13][1]
                    stats["maxPosRSizeNC"] = ex[14][1]
                    stats["numAtomOccC"] = ex[15][1]
                    stats["numAtomOccNC"] = ex[16][1]
                    stats["maxNumAtomOcc"] = ex[17][1]
                    stats["maxNumPosOccA"] = ex[18][1]
                    stats["maxNumNegOccA"] = ex[19][1]
                    stats["maxSizeNCon"] = ex[20][1]
                    stats["maxSizeHeadNegBodyRule"] = ex[21][1]
                    stats["maxSizeRuleHead"] = ex[22][1]
                    stats["maxSizePosBodyNCon"] = ex[23][1]
                    stats["maxSizeNegBodyRule"] = ex[24][1]
                    stats["maxSizePosBodyCon"] = ex[25][1]
                    stats["maxSizeNegBodyCon"] = ex[26][1]
                    stats["numAtomsHead"] = ex[27][1]
                    stats["numAtomsPosBody"] = ex[28][1]
                    stats["numAtomsNegBody"] = ex[29][1]
                    stats["maxNumVarOcc"] = ex[30][1]
                    stats["maxNumVarOccHeadNegBody"] = ex[31][1]
                    stats["maxSizeAnswerSet"] = ex[32][1]

                    # get Dependency Graph Stats
                    if gen_d_width:
                        print_message("      dependency Graph")
                        ret = gen_graph_stats(d['Stats']['Incidence Graph'])
                        if not ret[3]:
                            stats["dGraphWidth"] = ret[0]

                    # get Incidence Graph Stats
                    if gen_i_width:
                        print_message("      incidence Graph")
                        ret = gen_graph_stats(d['Stats']['Incidence Graph'])
                        if not ret[3]:
                            stats["iGraphWidth"] = ret[0]

                    # get ReductGraph Stats
                    if gen_r_width:
                        print_message("      reduct Graph")
                        for i in range(0, 10):
                            print_message("        " + str(i))
                            if len(d['Stats']['Reduct Graph']) > i:
                                ret = gen_graph_stats(d['Stats']['Reduct Graph'][i])
                                if not ret[3]:
                                    stats["rGraphWidth" + str(i)] = ret[0]

    with open(csvFile, "a") as statsFile:
        writer = csv.DictWriter(statsFile, fieldnames=names_csv, delimiter=',', lineterminator='\n')
        writer.writerow(stats)


# create missing program directories and files
def create_missing_dirs():
    if not isdir(dirPrograms):
        os.makedirs(dirPrograms)
    if not isdir(dirGround):
        os.makedirs(dirGround)
    if not isdir(dirGraphs):
        os.makedirs(dirGraphs)
    if not isdir(dirTimes):
        os.makedirs(dirTimes)
    if not isdir(dirExtendedStats):
        os.makedirs(dirExtendedStats)
    if not isfile(outputFile):
        open(outputFile, "w").close()
    if not isfile(logGroundOversize):
        open(logGroundOversize, "w").close()
    if not isfile(logGroundTimeout):
        open(logGroundTimeout, "w").close()
    if not isfile(logGroundError):
        open(logGroundError, "w").close()
    if not isfile(logTimesFailed):
        open(logTimesFailed, "w").close()
    if not isfile(logStatsFailed):
        open(logStatsFailed, "w").close()
    if not isfile(tmpFile):
        open(tmpFile, "w").close()


numElements = 0
countElements = 0
create_missing_dirs()

# ground the Programs
if groundPrograms:
    for pr in listdir(dirPrograms):
        for tdir in listdir(join(dirPrograms, pr)):
            numElements += len(listdir(join(join(dirPrograms, pr), tdir)))
    for year in listdir(dirPrograms):
        for problem in listdir(join(dirPrograms, year)):
            p_dir = join(dirPrograms, year, problem)
            for inst in listdir(p_dir):
                if inst != "encoding.asp" and not (inst in open(logGroundOversize).read()) and not (
                            inst in open(logGroundTimeout).read()):
                    countElements += 1
                    print_message("Ground Instance(" + str(countElements) + "/" + str(numElements) + "): " + inst)
                    try:
                        ground_programs(inst, p_dir)
                    except:
                        print_error("Grounding Error: " + inst)

# get clasp statistics
countElements = 0
if getClaspStatistics:
    numElements = len(listdir(dirGround))
    for inst in listdir(dirGround):
        countElements += 1
        print_message("clasp instance(" + str(countElements) + "/" + str(numElements) + "): " + inst)
        if isfile(join(dirGround, inst)) and not (inst in open(logTimesFailed).read()):
            print_message("get clasp Parameters")
            try:
                get_times(inst)
            except:
                print_error("__--error--__ " + inst)
        else:
            print_message("no clasp Parameters")

# get extended statistics
countElements = 0
if getExtendedStatistics:
    numElements = len(listdir(dirTimes))
    for inst in listdir(dirTimes):
        countElements += 1
        print_message("exst instance(" + str(countElements) + "/" + str(numElements) + "): " + inst)
        if isfile(join(dirTimes, inst)) and not (inst in open(logStatsFailed).read()):
            print_message("get extended Parameters")
            try:
                get_stats(inst)
            except:
                print_error("__--error--__ " + inst)
        else:
            print_message("no extended Parameters")

# generate the CSV
countElements = 0
if generateCSV:
    matches = {}
    for pr in listdir(dirPrograms):
        for tdir in listdir(join(dirPrograms, pr)):
            for prog in listdir(join(join(dirPrograms, pr), tdir)):
                matches[prog] = tdir

    numElements = len(listdir(dirExtendedStats))
    for inst in listdir(dirExtendedStats):
        countElements += 1
        print_message("csv instance(" + str(countElements) + "/" + str(numElements) + "): " + inst)
        if isfile(join(dirExtendedStats, inst)):
            get_csv(inst)
