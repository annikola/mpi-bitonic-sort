/******************************************************************************
* FILE: mpi_hello.c
* DESCRIPTION:
*   MPI tutorial example code: Simple hello world program
* AUTHOR: Blaise Barney
* LAST REVISED: 03/05/10
******************************************************************************/

#include "mpi.h"
#include "../include/vectoral_bitonic_sort.h"
#include <stdio.h>
#include <stdlib.h>

#define MASTER 0
#define MIN_ARGS 2
#define MIN_P 1
#define MAX_P 7
#define MIN_Q 20
#define MAX_Q 27

int log_base_2(int arg);

int main (int argc, char *argv[]) {

    int numtasks, taskid, len;
    int p, q, reps, total_reps;
    char hostname[MPI_MAX_PROCESSOR_NAME];

    if (argc < MIN_ARGS + 1) {
        printf("Missing %d argument(s)\n", MIN_ARGS + 1 - argc);
        return 1;
    }

    p = atoi(argv[1]);
    if (p < MIN_P || p > MAX_P) {
        printf("Please insert a value for p between %d and %d\n", MIN_P, MAX_P);
        return 1;
    }
    q = atoi(argv[2]);
    if (q < MIN_Q || q > MAX_Q) {
        printf("Please insert a value for q between %d and %d\n", MIN_Q, MAX_Q);
        return 1;
    }

    reps = p + 1;
    total_reps = ((reps - 1) * reps) / 2;

    mpi_bitonic_warmup(ipow(2, p), reps, total_reps);

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
    MPI_Get_processor_name(hostname, &len);
    printf ("Hello from task %d on %s!\n", taskid, hostname);
    if (taskid == MASTER) {
        printf("MASTER: Number of MPI tasks is: %d\n",numtasks);
    }
    MPI_Finalize();

    return 0;
}

int log_base_2(int arg) {

    int result;

    result = 0;
    while (arg > 1) {
        if (!(arg % 2)) {
            return -1;
        }
        arg /= 2;
        result++;
    }

    return result;
}
