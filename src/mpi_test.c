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

#define MAX_INTEGER 100

int asc_compare(const void *a, const void *b);
int desc_compare(const void *a, const void *b);
// void swap(int *a, int *b);
int array_compare(int *arr1, int *arr2, int n);
void elbow_sort(int *arr, int n, int flow);
void reverse(int *arr, int start, int end);
void rotate_left(int *arr, int n, int k);
void reform_bitonic(int *arr, int n);
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
    
    MPI_Barrier(MPI_COMM_WORLD);

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
    elbow_sort(A, Q, ASCENT);
    // printf("%d HAS ", taskid);
    // for (int m = 0; m < Q; m++) {
    //     printf("%d ", A[m]);
    //     B[m] = A[m];
    // }
    // printf("\n");

    qsort(A, Q, sizeof(int), asc_compare);
    if (array_compare(A, B, Q)) {
        printf("Correctly Sorted!\n");
    } else {
        printf("Falsely Sorted!\n");
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

void elbow_sort(int *arr, int n, int flow) {

    int i, t;
    int peak, low, high;
    int *temp;

    // MPOROUME PIO GRIGORA APO O(5*n/2 + n(pisw sto kanoniko array)) AN EXOUME DUPLICATES???
    
    reform_bitonic(arr, n);

    // Here we find the peak of the standard form bitonic
    // by taking advantage of the fact that we always start
    // traversing the not descending part
    peak = -1;
    for (i = 1; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            peak = i;
            break;
        }
    }
    
    // If no peak was found then the whole sequence
    // is either not ascending or not descending
    if (peak == -1 && arr[0] <= arr[n - 1]) {
        return;
    } else if (peak == -1 && arr[0] > arr[n - 1]) {
        reverse(arr, 0, n - 1);
        return;
    }

    // Temporary array to store the sorted elements
    temp = (int *)malloc(n * sizeof(int));

    // Merging both the sorted arrays before and after the peak element
    low = 0;
    high = n - 1;
    t = 0;
    while (low <= peak && high >= peak) {
        // Storing the smaller value element in the temp array
        if (arr[low] < arr[high]) {
            temp[t] = arr[low];
            t++;
            low++;
        } else {
            temp[t] = arr[high];
            t++;
            high--;
        }
    }
    temp[t] = arr[peak];

    // Storing all elements of the temporary array back in the original array
    if (!flow) {
        for (i = 0; i < n; i++) {
            arr[i] = temp[i];
        }
    } else {
        for (i = 0; i < n; i++) {
            arr[i] = temp[n - i - 1];
        }
    }

    free(temp);
}

void reform_bitonic(int *arr, int n) {

    int k, steep;

    steep = arr[0];
    k = 1;
    while (k < n - 1) {
        if (arr[k] < steep && arr[k] < arr[k + 1]) {
            rotate_left(arr, n, k);
            break;
        } else if (arr[k] < steep && arr[k] == arr[k + 1]) {
            k++;
        } else {
            steep = arr[k];
            k++;
        }
    }
}

void reverse(int *arr, int start, int end) {

    while (start < end) {
        int temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}

// Function to perform left rotation of the array by k positions
void rotate_left(int *arr, int n, int k) {

    reverse(arr, 0, k - 1);    // Reverse the first part
    reverse(arr, k, n - 1);    // Reverse the second part
    reverse(arr, 0, n - 1);    // Reverse the whole array
}

void bitonic_swap(int *v1, int *v2, int idx) {
    int dummy;

    dummy = v1[idx];
    v1[idx] = v2[idx];
    v2[idx] = dummy;
}
