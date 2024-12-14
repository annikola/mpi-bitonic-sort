#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define P 4
#define Q 4

#define MAX_INTEGER 100
#define MIN 0
#define MAX 1

int asc_compare(const void *a, const void *b);
int desc_compare(const void *a, const void *b);
void bitonic_swap(int *v1, int *v2, int idx);
void elbow_merge();

int main(int argc, char *argv[]) {

    int i, j, h, k;
    int A[P][Q + 1];
    int uno_reverse;

    srand(time(0));

    uno_reverse = 0;
    for (i = 0; i < P; i++) {
        for (j = 0; j < Q; j++) {
            A[i][j] = rand() % MAX_INTEGER + 1;
        }
        if ((i + uno_reverse) % 2 == 0) {
            A[i][Q] = MIN;
            uno_reverse = 0;
        } else {
            A[i][Q] = MAX;
            uno_reverse = 1;
        }
    }

    for (i = 0; i < P; i++) {
        for (j = 0; j < Q; j++) {
            printf("%d ", A[i][j]);
        }
        // printf("\n");
    }
    printf("\n");

    for (i = 0; i < P; i++) {
        if (i % 2 == 0) {
            qsort(A[i], Q, sizeof(int), asc_compare);
        } else {
            qsort(A[i], Q, sizeof(int), desc_compare);
        }
    }

    for (i = P / 2; i >= 1; i /= 2) {
        for (j = 0; j < i; j++) {
            for (h = 0; h < i; h++) {
                // elbow_merge();
                for (k = 0; k < Q; k++) {
                    if ((A[j][k] > A[j + 1][k] && A[j][Q] == MIN) || (A[j][k] < A[j + 1][k] && A[j][Q] == MAX)) {
                        bitonic_swap(A[j], A[j + 1], k);
                    }
                }
                if (j == 0) { // HMM...
                    qsort(A[j], Q, sizeof(int), asc_compare);
                    A[j][Q] = MIN;
                    qsort(A[j + 1], Q, sizeof(int), asc_compare);
                    A[j + 1][Q] = MIN;
                } else {
                    qsort(A[j], Q, sizeof(int), desc_compare);
                    A[j][Q] = MAX;
                    qsort(A[j + 1], Q, sizeof(int), desc_compare);
                    A[j + 1][j] = MAX;
                }
            }
        }
    }

    for (j = 0; j < P; j += 2) {
        for (k = 0; k < Q; k++) {
            if ((A[j][k] > A[j + 1][k] && A[j][Q] == MIN) || (A[j][k] < A[j + 1][k] && A[j][Q] == MAX)) {
                bitonic_swap(A[j], A[j + 1], k);
            }
        }
        if (j == 0) { // HMM...
            qsort(A[j], Q, sizeof(int), asc_compare);
            A[j][Q] = MIN;
            qsort(A[j + 1], Q, sizeof(int), asc_compare);
            A[j + 1][Q] = MIN;
        } else {
            qsort(A[j], Q, sizeof(int), desc_compare);
            A[j][Q] = MAX;
            qsort(A[j + 1], Q, sizeof(int), desc_compare);
            A[j + 1][j] = MAX;
        }
        // EDW PREPEI NA ANNEWTHOUN TA MIN MAX!!!
    }

    for (j = 0; j < P / 2; j++) {
        for (k = 0; k < Q; k++) {
            if (A[j][k] > A[j + 2][k] && A[j][Q] == MIN) {
                bitonic_swap(A[j], A[j + 2], k);
            }
        }
        if (j == 0) { // HMM...
            A[j][Q] = MIN;
            A[j + 2][Q] = MIN;
        } else {
            A[j][Q] = MAX;
            A[j + 2][Q] = MAX;
        }
        // EDW PREPEI NA ANANEWTHOUN TA MIN MAX!!!
    }

    for (j = 0; j < P; j += 2) {
        for (k = 0; k < Q; k++) {
            if (A[j][k] > A[j + 1][k] && A[j][Q] == MIN) {
                bitonic_swap(A[j], A[j + 1], k);
            }
        }
        qsort(A[j], Q, sizeof(int), asc_compare);
        qsort(A[j + 1], Q, sizeof(int), asc_compare);
    }

    printf("\n");
    for (i = 0; i < P; i++) {
        for (j = 0; j < Q; j++) {
            printf("%d ", A[i][j]);
        }
        // printf("\n");
    }
    printf("\n");

    return 0;
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

void elbow_merge() {
    ;
}
