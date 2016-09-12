from os import listdir
import collections

# path to the folder with the ground test data files
testData = ["./Ground/Split/1s", "./Ground/Split/10s", "./Ground/Split/100s", "./Ground/Split/1000s"]

# directory for the result
numElements = 0
for tdir in testData:
    numElements += len(listdir(tdir))

resultDir = "./Ground/Split"
countElements = 0

allFiles = []

for tdir in testData:
    allFiles += listdir(tdir)

print "Duplicates:"
duplicates = [item for item, count in collections.Counter(allFiles).items() if count > 1]
for d in duplicates:
    print d
