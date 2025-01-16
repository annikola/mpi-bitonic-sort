# mpi-bitonic-sort
Parallel &amp; Distributed Computer Systems - Homework 2

Instructions in order to run the implementation on Aristotelis:

1) After you login to Aristotelis, execute the following in the same order as below:
```
module load gcc/9.2.0
module load openmpi/3.1.3
```
2) ```make all```
3) Run the .sbatch file like so:
```
sbatch mpi_bitonic_sort.sbatch
```
You can edit it to test on a different partition or using a different configuration of nodes and tasks-per-node.

4) If anything goes wrong and you can not immediately solve it please contact me on Zulip. Thank you!
