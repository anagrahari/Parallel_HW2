#!/bin/bash
#SBATCH -J par_rand_lj           # job name
#SBATCH -o par_rand_lj.o%j       # output and error file name (%j expands to jobID)
#SBATCH -n 1               # total number of mpi tasks requested
#SBATCH -p development     	   # queue (partition) -- normal, development, etc.
#SBATCH -t 02:00:00        # run time (hh:mm:ss) - 2 hours
#SBATCH --mail-type=begin  # email me when the job starts
#SBATCH --mail-type=end    # email me when the job finishes
p=16
rm -f randomized_time
touch randomized_time
while [ $p -gt 0 ]
do
	export CILK_NWORKERS=$p
	./par_randomized_cc 0 < input/com-lj-in.txt >> randomized_time
	p=`expr $p - 1`
done
