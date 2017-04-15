#!/bin/bash
#SBATCH -J par_rand_jobs           # job name
#SBATCH -o par_rand_jobs.o%j       # output and error file name (%j expands to jobID)
#SBATCH -n 1               # total number of mpi tasks requested
#SBATCH -p development     	   # queue (partition) -- normal, development, etc.
#SBATCH -t 02:00:00        # run time (hh:mm:ss) - 2 hours
#SBATCH --mail-user=muksharma@cs.stonybrook.edu
#SBATCH --mail-type=begin  # email me when the job starts
#SBATCH --mail-type=end    # email me when the job finishes
./par_randomized_cc.sh input/as-skitter-in.txt rand_output/as-skitter
./par_randomized_cc.sh input/ca-AstroPh-in.txt rand_output/ca-AstroPh
./par_randomized_cc.sh input/com-amazon-in.txt rand_output/com-amazon
./par_randomized_cc.sh input/com-dblp-in.txt rand_output/com-dblp
./par_randomized_cc.sh input/com-lj-in.txt rand_output/com-lj
./par_randomized_cc.sh input/com-orkut-in.txt rand_output/com-orkut
./par_randomized_cc.sh input/roadNet-CA-in.txt rand_output/roadNet-CA
./par_randomized_cc.sh input/roadNet-PA-in.txt rand_output/roadNet-PA
./par_randomized_cc.sh input/roadNet-TX-in.txt rand_output/roadNet-TX
./par_randomized_cc.sh input/com-friendster-in.txt rand_output/com-friendster
