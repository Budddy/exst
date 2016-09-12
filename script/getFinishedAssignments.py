from os import listdir
from os.path import isfile, join
import json
import shutil

# path to the folder with the ground test data files
testData = ["./Result/result_assignments"]

# directory for the result
numElements = 0
for tdir in testData:
    numElements += len(listdir(tdir))

resultDir = "./Ground/Stats"
dataDir = "./Ground/All"
countElements = 0

for tdir in testData:
    for name in listdir(tdir):
        countElements += 1
        path = join(tdir, name)
        print ("\ninstance(" + str(countElements) + "/" + str(numElements) + "): " + name)
        if isfile(path) and isfile(join(dataDir, name)):
            if not (isfile(join(resultDir, name)) or (name == "_result_")):
                shutil.copyfile(join(dataDir, name), join(resultDir, name))
