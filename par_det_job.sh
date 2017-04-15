#!/bin/bash
#SBATCH -J par_det_jobs           # job name
#SBATCH -o par_det_jobs.o%j       # output and error file name (%j expands to jobID)
#SBATCH -n 1               # total number of mpi tasks requested
#SBATCH -p dev     	   # queue (partition) -- normal, development, etc.
#SBATCH -t 02:00:00        # run time (hh:mm:ss) - 2 hours
#SBATCH --mail-user=muksharma@cs.stonybrook.edu
#SBATCH --mail-type=begin  # email me when the job starts
#SBATCH --mail-type=end    # email me when the job finishes
./par_deterministic_cc.sh input/as-skitter-in.txt deterministic_output/as-skitter
./par_deterministic_cc.sh input/ca-AstroPh-in.txt deterministic_output/ca-AstroPh
./par_deterministic_cc.sh input/com-amazon-in.txt deterministic_output/com-amazon
./par_deterministic_cc.sh input/com-dblp-in.txt deterministic_output/com-dblp
./par_deterministic_cc.sh input/com-lj-in.txt deterministic_output/com-lj
./par_deterministic_cc.sh input/com-orkut-in.txt deterministic_output/com-orkut
./par_deterministic_cc.sh input/roadNet-CA-in.txt deterministic_output/roadNet-CA
./par_deterministic_cc.sh input/roadNet-PA-in.txt deterministic_output/roadNet-PA
./par_deterministic_cc.sh input/roadNet-TX-in.txt deterministic_output/roadNet-TX
./par_deterministic_cc.sh input/com-friendster-in.txt deterministic_output/com-friendster
