from os import listdir
from os.path import isfile, join
import json
import shutil

# path to the folder with the ground test data files
testData = "./"

# directory for the result
resultDir = "../Separated"
numElements = len(listdir(testData))
countElements = 0

for name in listdir(testData):
    countElements += 1
    path = join(testData, name)
    if isfile(path):
        print ("\ninstance(" + str(countElements) + "/" + str(numElements) + "): " + name)
        f = open(path, 'r')
        data = f.read()
        j = json.loads(data)
        s = j['Time']['CPU']
        if (s <= 1):
            shutil.copyfile(path, join(join(resultDir, '1s'), name[6:-9]))
        elif s <= 10:
            shutil.copyfile(path, join(join(resultDir, '10s'), name[6:-9]))
        elif s <= 100:
            shutil.copyfile(path, join(join(resultDir, '100s'), name[6:-9]))
        elif s <= 600:
            shutil.copyfile(path, join(join(resultDir, '600s'), name[6:-9]))
