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
void swap(int *a, int *b);
void sortArr(int *arr, int n);
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
    // printf("%d HAS ", taskid);
    // for (int m = 0; m < Q; m++) {
    //     printf("%d ", A[m]);
    // }
    // printf("\n");
    // printf("\n");
    MPI_Barrier(MPI_COMM_WORLD);

    for (i = 0; i < total_reps; i++) {
        if (instructions[i][taskid].sort == ASCENT && i == 0) {
            qsort(A, Q, sizeof(int), asc_compare);
        } else if (instructions[i][taskid].sort == ASCENT && i > 0){
            reform_bitonic(A, Q);
            sortArr(A, Q);
        } else if (instructions[i][taskid].sort == DESCENT && i == 0) {
            qsort(A, Q, sizeof(int), desc_compare);
        } else if (instructions[i][taskid].sort == DESCENT && i > 0) {
            reform_bitonic(A, Q);
            sortArr(A, Q);
            qsort(A, Q, sizeof(int), desc_compare);
        }
        // printf("%d HAS ", taskid);
        // for (int m = 0; m < Q; m++) {
        //     printf("%d ", A[m]);
        // }
        // printf("i = %d\n", i);

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
        // printf("%d HAS ", taskid);
        // for (int m = 0; m < Q; m++) {
        //     printf("%d ", A[m]);
        // }
        // printf("i = %d\n", i);
    }

    // printf("sort elements of %d ascending\n", taskid);
    reform_bitonic(A, Q);
    sortArr(A, Q);
    printf("%d HAS ", taskid);
    for (int m = 0; m < Q; m++) {
        printf("%d ", A[m]);
    }
    printf("\n");

    MPI_Finalize();

    return 0;
}

int asc_compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int desc_compare(const void *a, const void *b) {
    return (*(int *)b - *(int *)a);
}

void sortArr(int* arr, int n) {

    // Temporary array to store the sorted elements
    int *tmp = (int *)malloc(n * sizeof(int));
    
    // Index of peak element in the bitonic array
    int peak = -1;
    int low = 0;
    int high = n - 1;
    int k = 0;
 
    // Modified Binary search to find the index of peak element
    while (low <= high){
 
        int mid = (low + high) / 2;
 
        // Condition for checking element at index mid is peak element or not
        if (( mid == 0 || arr[mid - 1] <= arr[mid] ) && 
            ( mid == n - 1 || arr[mid + 1] < arr[mid] )){
            peak = mid;
            break;
        }

        // If elements before mid element
        // are in increasing order it means
        // peak is present after the mid index
        if (arr[mid] < arr[mid + 1])
            low = mid + 1;
             
        // If elements after mid element
        // are in decreasing order it means
        // peak is present before the mid index
        else
            high = mid - 1;
    }
    // printf("PEAK: %d\n", arr[peak]);

    // Merging both the sorted arrays before and after the peak element
    low = 0;
    high = n - 1;

    // Loop until any of both arrays is exhausted
    while (low <= peak && high > peak) {
        // Storing the smaller value element in the tmp array
        if (arr[low] < arr[high]) {
            tmp[k] = arr[low];
            k++;
            low++;
        } else {
            tmp[k] = arr[high];
            k++;
            high--;
        }
    }

    // Storing remaining elements of the array which are present before the peak element in tmp array
    while (low <= peak) {
        tmp[k] = arr[low];
        k++;
        low++;
    }

    // Storing remaining elements of the array which are present after the peak element in tmp array
    while (high > peak) {
        tmp[k] = arr[high];
        k++;
        high--;
    }

    // Storing all elements of the tmp array back in the original array
    for (int i = 0; i < n; i++) {
        arr[i] = tmp[i];
    }

    free(tmp);
}

void reform_bitonic(int *arr, int n) {

    int i;

    for (i = 1; i < n - 1; i++) {
        if (arr[i] <= arr[i - 1] && arr[i] < arr[i + 1]) {
            rotate_left(arr, n, i);
            break;
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
