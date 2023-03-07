#!/bin/bash -x
#SBATCH --gres=gpu:1
#SBATCH --time=00:02:00
#SBATCH --partition=el8
#SBATCH --ntasks=160
#SBATCH --nodes=8


module load spectrum-mpi


# taskset -c 0-159:4 mpirun main
taskset -c 0-159:$1 mpirun -N $2 /gpfs/u/home/PCPC/PCPCdrws/scratch/Parallel-Homework-3/a.out