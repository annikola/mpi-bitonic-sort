#! /usr/bin/env bash

#SBATCH --partition=batch
#SBATCH --ntasks-per-node=8
#SBATCH --nodes=4
#SBATCH --time=00:05:00

module load gcc/9.2.0
module load openmpi/3.1.3

echo "Job started at $(date)"
echo "Running on nodes: $SLURM_NODELIST"

srun -n 32 build/mpi_bitonic_sort 5 27

echo "Job finished at $(date)"
