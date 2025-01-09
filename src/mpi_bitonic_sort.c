#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/vectoral_bitonic_sort.h"
#include "../include/bitonic_sequence_helpers.h"

#define MIN_ARGS 2
#define MIN_P 1
#define MAX_P 7
#define MIN_Q 20
#define MAX_Q 27

int asc_compare(const void *a, const void *b);
int desc_compare(const void *a, const void *b);
int array_compare(int *arr1, int *arr2, int n);
void swap(int *a, int *b);
void exchange(int *A, int *B, int n, int minmax);

int main (int argc, char *argv[]) {

    int i, k;
    int taskid;
    int p, q, Q, total_proc, reps, total_reps;
    int *A, *B;
    int validator;
    double start_time, end_time, total_local_time, max_time;
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

    start_time = MPI_Wtime();

    srand(time(0) + taskid);

    // Calculate the desired instructions table based on the requirements
    Q = ipow(2, q);
    total_proc = ipow(2, p);
    reps = p + 1;
    total_reps = ((reps - 1) * reps) / 2;
    instructions = mpi_bitonic_warmup(total_proc, reps, total_reps);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);

    B = (int *)malloc(Q * sizeof(int));
    A = (int *)malloc(Q * sizeof(int));
    for (i = 0; i < Q; i++) {
        A[i] = rand();
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // Perform the main exchange and sorting based on the generated instructions
    for (i = 0; i < total_reps; i++) {

        if (instructions[i][taskid].sort == ASCENT && i == 0) {
            qsort(A, Q, sizeof(int), asc_compare);
        } else if (instructions[i][taskid].sort == ASCENT && i > 0){
            elbow_sort(A, Q, ASCENT);
        } else if (instructions[i][taskid].sort == DESCENT && i == 0) {
            qsort(A, Q, sizeof(int), desc_compare);
        } else if (instructions[i][taskid].sort == DESCENT && i > 0) {
            elbow_sort(A, Q, DESCENT);
        }

        if (!instructions[i][taskid].flow) {
            MPI_Send(A, Q, MPI_INT, instructions[i][taskid].target_pid, 0, MPI_COMM_WORLD);
            MPI_Recv(B, Q, MPI_INT, instructions[i][taskid].target_pid, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            exchange(A, B, Q, instructions[i][taskid].flow);
        } else {
            MPI_Recv(B, Q, MPI_INT, instructions[i][taskid].target_pid, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(A, Q, MPI_INT, instructions[i][taskid].target_pid, 0, MPI_COMM_WORLD);
            exchange(A, B, Q, instructions[i][taskid].flow);
        }
    }

    // Perform the final sort locally
    elbow_sort(A, Q, ASCENT);
    MPI_Barrier(MPI_COMM_WORLD);
    end_time = MPI_Wtime();
    total_local_time = end_time - start_time;

    // Get the maximum finishing time of of all tasks (finishing time)
    MPI_Reduce(&total_local_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, total_proc - 1, MPI_COMM_WORLD);
    if (taskid == total_proc - 1) {
        printf("Total execution time: %lf seconds\n", max_time);
    }

    // Check for local ascent
    for (i = 0; i < Q; i++) {
        B[i] = A[i];
    }
    qsort(A, Q, sizeof(int), asc_compare);
    if (!array_compare(A, B, Q)) {
        printf("Falsely Sorted!\n");
    }

    // Check for global ascent
    if (taskid == 0) {
        MPI_Send(&A[Q - 1], 1, MPI_INT, taskid + 1, 0, MPI_COMM_WORLD);
    } else if (taskid == total_proc - 1) {
        MPI_Recv(&validator, 1, MPI_INT, taskid - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (validator <= A[Q - 1]) {
            printf("Correctly Sorted!\n");
        } else {
            printf("Falsely Sorted!\n");
        }
    } else {
        MPI_Recv(&validator, 1, MPI_INT, taskid - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (validator > A[Q - 1]) {
            printf("Falsely Sorted!\n");
        }
        MPI_Send(&A[Q - 1], 1, MPI_INT, taskid + 1, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}

int asc_compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int desc_compare(const void *a, const void *b) {
    return (*(int *)b - *(int *)a);
}

int array_compare(int *arr1, int *arr2, int n) {

    int i;

    for (i = 0; i < n; i++) {
        if (arr1[i] != arr2[i]) {
            return 0;
        }
    }

    return 1;
}

void swap(int *a, int *b) {

    int dummy;

    dummy = *a;
    *a = *b;
    *b = dummy;
}

void exchange(int *A, int *B, int n, int minmax) {

    int i;

    for (i = 0; i < n; i++) {
        if (A[i] > B[i] && !minmax) {
            swap(&A[i], &B[i]);
        } else if (A[i] < B[i] && minmax) {
            swap(&A[i], &B[i]);
        }
    }
}
