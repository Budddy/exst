# exst

![License](http://img.shields.io/badge/license-GPLv3-blue.svg)
![Build Status Travis-CI](https://travis-ci.org/Budddy/additional-stats.svg?branch=master)

An extension for clasp to create additional paramters.

## Requirements:
- htd 1.0.1
- cmake 3.2.2
- dogygen 
  - to build the documentation
- Linux:
  - libc 2.23
- Windows:
  - mingw-w64
- Testing:
  - cppunit 1.12 newer versions don't work

## Compiling:
- Linux
  - To compile the extension run `cmake <PATH_TO_EXST_DIRECTORY>` and then `make`.
- Windows
  - To compile the extension run `cmake -G "MinGW Makefiles" <PATH_TO_EXST_DIRECTORY>` and then `mingw32-make`. Then put the htd dll library in the same folder as the exst binary.
- Tests
  - Linux
    - To compile the tests run `cmake -Dtests=ON <PATH_TO_EXST_DIRECTORY>` and then `make`.

## Options:
- `--printDgraph=<format>[,<filePath>]`
  Enables the output of the dependency graph either into a file or if no file path is provided onto standard output.
- `--printIgraph=<format>[,<filePath>]`
  Enables the output of the incidence graph into the given file or if no file path is provided the graph is written onto standard output.
- `--printRgraph=<format>[,<filePath>]`
  Enables the output of the incidence graph of the GL-reduct into a file or in case no file path is provided the graph is printed onto standard output. If a path is given, each reduct graph is printed into a separated file, therefore the name of each file is extended by the number of the reduct.  
- `--calcTWidth`
  Enables the treewidth generation of the GL-reduct.  
- `--width-intervall=<n>`
  This parameter provides the number of assignments between each reduct generation for the extension.  

### Graph Formats
The format 0 is for the dimacs graph format.
The format 1 is for the gr graph format, which is used in the pace challenge.
The format 2 is for the gml graph format.

## The additional paramters are:
- the number of nodes in the Dependency Graph
- the number of edges in the Dependency Graph
- the tree-width of the Dependency Graph
- the number of nodes in the Incidence Graph
- the number of edges in the Incidence Graph
- the tree-width of the Incidence Graph
- the number of nodes in the Incidence Graph of the GL-Reduct
- the number of edges in the Incidence Graph of the GL-Reduct
- the tree-width of the Incidence Graph of the GL-Reduct
- the number of non Horn Clauses
- the number of non Dual Horn Clauses
- the maximal clause size
- the maximal positive clause size
- the maximal negative clause size
- the number of variables that occur as positive literals with helpers
- the number of variables that occur as positive literals without helpers
- the number of variables that occur as negative literals with helpers
- the number of variables that occur as negative literals without helpers
- the maximal positive rule size constraint
- the maximal positive rule size non-constraint
- the total number of atom occurrences constraint
- the total number of atom occurrences non-constraint
- the maximal number of occurrences of an atom
- the maximal number of positive occurrences of an atom
- the maximal number of negative occurrences of an atom
- the maximal size of a non-constraint rule
- the maximal size of the head and negative body of a rule
- the maximal size of the head of a rule
- the maximal size of the positive body of a non-constraint rule
- the maximal size of the negative body of a rule
- the maximal size of the positive body of a constraint
- the maximal size of the negative body of a constraint
- the number of atoms that occur in the head
- the number of atoms that occur in the positive body
- the number of atoms that occur in the negative body
- the maximal number of occurences of a variable
- the maximal number of occurrences of a variable only head and negative body occurences
- the maximal size of the model