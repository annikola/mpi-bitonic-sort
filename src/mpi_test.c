/******************************************************************************
* FILE: mpi_hello.c
* DESCRIPTION:
*   MPI tutorial example code: Simple hello world program
* AUTHOR: Blaise Barney
* LAST REVISED: 03/05/10
******************************************************************************/

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/vectoral_bitonic_sort.h"

#define MASTER 0
#define MIN_ARGS 2
#define MIN_P 1
#define MAX_P 7
#define MIN_Q 2
#define MAX_Q 27

#define MAX_INTEGER 100000

int log_base_2(int arg);
int asc_compare(const void *a, const void *b);
int desc_compare(const void *a, const void *b);
void bitonic_swap(int *v1, int *v2, int idx);

int main (int argc, char *argv[]) {

    int t, i, j, k;
    int numtasks, taskid, len;
    int p, q, Q, total_proc, reps, total_reps;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    int *A, *B;
    Instruction **instructions;

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

    srand(time(0) + taskid);

    Q = ipow(2, q);
    total_proc = ipow(2, p);
    reps = p + 1;
    total_reps = ((reps - 1) * reps) / 2;
    instructions = mpi_bitonic_warmup(total_proc, reps, total_reps);

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    MPI_Get_processor_name(hostname, &len);
    // printf ("Hello from task %d on %s!\n", taskid, hostname);

    B = (int *)malloc(Q * sizeof(int));
    A = (int *)malloc(Q * sizeof(int));
    for (i = 0; i < Q; i++) {
        A[i] = rand() % MAX_INTEGER + 1;
    }
    // printf("%d HAS ", taskid);
    // for (int m = 0; m < Q; m++) {
    //     printf("%d ", A[m]);
    // }
    // printf("\n");
    MPI_Barrier(MPI_COMM_WORLD);

    for (i = 0; i < total_reps; i++) {
        if (instructions[i][taskid].sort == ASCENT) {
            // printf("sort elements of %d ascending\n", taskid);
            qsort(A, Q, sizeof(int), asc_compare);
        } else if (instructions[i][taskid].sort == DESCENT) {
            // printf("sort elements of %d descending\n", taskid);
            qsort(A, Q, sizeof(int), desc_compare);
        }

        if (!instructions[i][taskid].flow) {
            MPI_Send(A, Q, MPI_INT, instructions[i][taskid].target_pid, 0, MPI_COMM_WORLD);
            // printf("%d SENT TO %d at %ld\n", taskid, instructions[i][taskid].target_pid, clock());
            MPI_Recv(B, Q, MPI_INT, instructions[i][taskid].target_pid, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (k = 0; k < Q; k++) {
                if (A[k] > B[k]) {
                    bitonic_swap(A, B, k);
                }
            }
        } else {
            MPI_Recv(B, Q, MPI_INT, instructions[i][taskid].target_pid, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(A, Q, MPI_INT, instructions[i][taskid].target_pid, 0, MPI_COMM_WORLD);
            for (k = 0; k < Q; k++) {
                if (A[k] < B[k]) {
                    bitonic_swap(A, B, k);
                }
            }
            // printf("%d SENT TO %d at %ld\n", taskid, instructions[i][taskid].target_pid, clock());
        }
    }

    // printf("sort elements of %d ascending\n", taskid);
    qsort(A, Q, sizeof(int), asc_compare);
    printf("%d HAS ", taskid);
    for (int m = 0; m < Q; m++) {
        printf("%d ", A[m]);
    }
    printf("\n");

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

int asc_compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int desc_compare(const void *a, const void *b) {
    return (*(int *)b - *(int *)a);
}

void bitonic_swap(int *v1, int *v2, int idx) {
    int dummy;

    dummy = v1[idx];
    v1[idx] = v2[idx];
    v2[idx] = dummy;
}
