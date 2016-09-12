from os import listdir
from os.path import isfile, join
import json
import shutil

# path to the folder with the ground test data files
testData = ["./Result/result_1s", "./Result/result_10s", "./Result/result_100s", "./Result/result_1000s"]

# directory for the result
numElements = 0
for tdir in testData:
    numElements += len(listdir(tdir))

resultDir = "./Ground/Split"
dataDir = "./Ground/All"
countElements = 0

for tdir in testData:
    for name in listdir(tdir):
        countElements += 1
        path = join(tdir, name)
        if isfile(path) and isfile(join(dataDir, name)):
            print ("\ninstance(" + str(countElements) + "/" + str(numElements) + "): " + name)
            f = open(path, 'r')
            data = f.read()
            j = json.loads(data)
            r = j['Result']
            if r != 'UNSATISFIABLE' and r != 'UNKNOWN' and not (
                        (isfile(join(join(resultDir, '1s'), name))) or (isfile(join(join(resultDir, '10s'), name))) or (
                isfile(join(join(resultDir, '100s'), name))) or (isfile(join(join(resultDir, '1000s'), name)))):
                s = j['Time']['CPU']
                if s <= 1:
                    shutil.copyfile(join(dataDir, name), join(join(resultDir, '1s'), name))
                elif s <= 10:
                    shutil.copyfile(join(dataDir, name), join(join(resultDir, '10s'), name))
                elif s <= 100:
                    shutil.copyfile(join(dataDir, name), join(join(resultDir, '100s'), name))
                elif s <= 1000:
                    shutil.copyfile(join(dataDir, name), join(join(resultDir, '1000s'), name))
