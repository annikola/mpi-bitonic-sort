#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define P 16
#define Q 4
#define REPS 5 // log(P) + 1
#define TOTAL_REPS ((REPS - 1) * REPS / 2)

#define MAX_INTEGER 100
#define MIN 0
#define MAX 1
#define ASCENT 0
#define DESCENT 1

int ipow(int base, int exp);
int asc_compare(const void *a, const void *b);
int desc_compare(const void *a, const void *b);
void bitonic_swap(int *v1, int *v2, int idx);
void elbow_merge();

int main(int argc, char *argv[]) {

    int i, j, t, h, m, n, k, s;
    int A[P][Q + 1];
    int uno_reverse, repetition, minmax_size, step, position;
    int ascento[REPS][P];
    int used[P];
    char minmax[TOTAL_REPS][P];

    srand(time(0));

    // uno_reverse = 0;
    // for (i = 0; i < P; i++) {
    //     for (j = 0; j < Q; j++) {
    //         A[i][j] = rand() % MAX_INTEGER + 1;
    //     }
    //     if ((i + uno_reverse) % 2 == 0) {
    //         A[i][Q] = MIN;
    //         uno_reverse = 0;
    //     } else {
    //         A[i][Q] = MAX;
    //         uno_reverse = 1;
    //     }
    // }

    repetition = 0;
    for (i = 1; i <= P; i *= 2) {
        uno_reverse = 0;
        for (j = 0; j < P; j += i) {
            for (k = 0; k < i; k++) {
                if (uno_reverse % 2 == 0) {
                    ascento[repetition][j + k] = ASCENT;
                } else {
                    ascento[repetition][j + k] = DESCENT;
                }
            }
            uno_reverse++;
        }
        repetition++;
    }

    position = 0;
    for (repetition = 0; repetition < REPS - 1; repetition++) {
        for (i = 0; i < P; i++) {
            if (!ascento[repetition][i] && !ascento[repetition + 1][i]) {
                // printf("MIN ");
                minmax[position][i] = MIN;
            } else if (!ascento[repetition][i] && ascento[repetition + 1][i]) {
                // printf("MAX ");
                minmax[position][i] = MAX;
            } else if (ascento[repetition][i] && !ascento[repetition + 1][i]) {
                // printf("MAX ");
                minmax[position][i] = MAX;
            } else {
                // printf("MIN ");
                minmax[position][i] = MIN;
            }
        }
        position++;
        // printf("\n");

        // printf("repetition: %d\n", repetition);
        // step = ipow(2, repetition); // half elbow
        for (j = 0; j < repetition; j++) {
            step = ipow(2, repetition - j) / 2;
            for (i = 0; i < P; i++) {
                used[i] = 0;
            }
            for (i = 0; i < P; i++) {
                if (!used[i]) {
                    if (!ascento[repetition + 1][i]) {
                        minmax[position][i] = MIN;
                        minmax[position][i + step] = MAX;
                        // printf("MIN%d MAX%d ", i, i + step);
                    } else {
                        minmax[position][i] = MAX;
                        minmax[position][i + step] = MIN;
                        // printf("MAX%d MIN%d ", i, i + step);
                    }
                    // printf("%d with %d\n", i, i + step);
                    used[i] = 1;
                    used[i + step] = 1;
                }
            }
            position++;
            // printf("\n");
        }
    }

    // for (i = 0; i < REPS; i++) {
    //     for (j = 0; j < P; j++) {
    //         printf("%d ", ascento[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    for (i = 0; i < TOTAL_REPS; i++) {
        for (j = 0; j < P; j++) {
            if (!minmax[i][j]) {
                printf("MIN ");
            } else {
                printf("MAX ");
            }
        }
        printf("\n\n");
    }

    // printf("\n");
    // for (i = 0; i < P; i++) {
    //     for (j = 0; j < Q; j++) {
    //         printf("%d ", A[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    return 0;
}

int ipow(int base, int exp) {
    int result = 1;

    for (;;) {
        if (exp & 1) {
            result *= base;
        }
        exp >>= 1;
        if (!exp) {
            break;
        }
        base *= base;
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

void elbow_merge() {
    ;
}
