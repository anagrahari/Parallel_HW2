#!/bin/bash
#SBATCH -J par_frnd           # job name
#SBATCH -o par_frnd.o%j       # output and error file name (%j expands to jobID)
#SBATCH -n 1               # total number of mpi tasks requested
#SBATCH -p normal    	   # queue (partition) -- normal, development, etc.
#SBATCH -t 05:00:00        # run time (hh:mm:ss) - 2 hours
#SBATCH --mail-user=muksharma@cs.stonybrook.edu
#SBATCH --mail-type=begin  # email me when the job starts
#SBATCH --mail-type=end    # email me when the job finishes
./par_deterministic_cc.sh input/com-friendster-in.txt deterministic_output/com-friendster-normal
./par_deterministic_cc_optimized.sh input/com-friendster-in.txt det_opt_output/com-friendster-normal
./par_randomized_cc.sh input/com-friendster-in.txt rand_output/com-friendster-normal
