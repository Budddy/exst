#!/usr/bin/env bash

# usefull condor commands
condor_q
condor_status
condor_submit
chmod u+x
ssh elvis
condor_rm mzisser

# runJob
executable              = execute.sh
Universe                = vanilla
log                     = run.log
output                  = outfile.txt
error                   = errors.txt
should_transfer_files   = NO
queue

# apply patch
patch -p1 < ./patches/clasp.patch

# generate patch
diff -rupN clasp-3.1.4/ clasp-3.1.4_patched/ > clasp.patch

# local python environment
/home/mzisser/virtualPython/virtualenv-15.0.1/myVE/bin/python2.7

# empty all files:
truncate -s 0 ./*

# download csv from elvis
scp -r ~/CSV_1/stats.csv mzisser@acrux.dbai.tuwien.ac.at:~/CSVs/stats_1.csv
scp -r ~/CSV_2/stats.csv mzisser@acrux.dbai.tuwien.ac.at:~/CSVs/stats_2.csv
scp -r ~/CSV_3/stats.csv mzisser@acrux.dbai.tuwien.ac.at:~/CSVs/stats_3.csv
scp -r ~/CSV_4/stats.csv mzisser@acrux.dbai.tuwien.ac.at:~/CSVs/stats_4.csv
scp -r ~/CSV_5/stats.csv mzisser@acrux.dbai.tuwien.ac.at:~/CSVs/stats_5.csv

# copy to elvis
scp -r ~/ mzisser@128.131.111.215:~/