#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define P 8
#define Q 4
#define REPS 4 // log(P) + 1

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
    int uno_reverse, pad, repetition, minmax_size, step;
    int ascento[REPS][P];
    char **minmax;

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

    minmax_size = 0;
    for (repetition = 0; repetition < REPS; repetition++) {
        minmax_size += repetition;
    }
    printf("minmax_size: %d\n", minmax_size);

    minmax = (char **)malloc(minmax_size * sizeof(char *));
    for (repetition = 0; repetition < REPS; repetition++) {
        minmax[repetition] = (char *)malloc(P * sizeof(char));
    }

    for (j = 0; j < REPS - 1; j++) {
        for (i = 0; i < P; i++) {
            if (!ascento[j][i] && !ascento[j + 1][i]) {
                printf("MIN ");
                minmax[j][i] = MIN;
            } else if (!ascento[j][i] && ascento[j + 1][i]) {
                printf("MAX ");
                minmax[j][i] = MAX;
            } else if (ascento[j][i] && !ascento[j + 1][i]) {
                printf("MAX ");
                minmax[j][i] = MAX;
            } else {
                printf("MIN ");
                minmax[j][i] = MIN;
            }
        }
        printf("\n");
    }

    for (repetition = 0; repetition < REPS; repetition++) {
        step = ipow(2, repetition);
        while ((step /= 2) >= 1) {
            for (i = 0; i < P; i += step) {
                printf("%d with %d\n", i, i + step);
            }
        }
    }

    // for (i = 0; i < REPS; i++) {
    //     for (j = 0; j < P; j++) {
    //         printf("%d ", ascento[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    // for (i = 0; i < P; i++) {
    //     if (i % 2 == 0) {
    //         qsort(A[i], Q, sizeof(int), asc_compare);
    //     } else {
    //         qsort(A[i], Q, sizeof(int), desc_compare);
    //     }
    // }

    // for (i = P / 2; i >= 1; i /= 2) {
    //     uno_reverse = 0;
    //     for (j = 0; j < i; j++) {
    //         printf("j = %d\n", j);
    //         pad = P / (2 * i); // the "elbow" length divided by 2
    //         if ((j + uno_reverse) % 2 == 0) {
    //             for (t = 0; t < pad; t++) {
    //                 A[t + j * (P / i)][Q] = MIN;
    //             }
    //             for (t = pad; t < 2*pad; t++) {
    //                 A[t + j * (P / i)][Q] = MAX;
    //             }
    //             uno_reverse = 0;
    //         } else {
    //             for (t = 0; t < pad; t++) {
    //                 A[t + j * (P / i)][Q] = MAX;
    //             }
    //             for (t = pad; t < 2*pad; t++) {
    //                 A[t + j * (P / i)][Q] = MIN;
    //             }
    //             uno_reverse = 1;
    //         }

    //         for (h = 0; h < pad; h++) {
    //             for (k = 0; k < Q; k++) {
    //                 if ((A[h + j * (P / i)][k] > A[h + j * (P / i) + pad][k] && A[h + j * (P / i)][Q] == MIN) || (A[h + j * (P / i)][k] < A[h + j * (P / i) + pad][k] && A[h + j * (P / i)][Q] == MAX)) {
    //                     bitonic_swap(A[h + j * (P / i)], A[h + j * (P / i) + pad], k);
    //                 }
    //             }
    //         }

    //         while ((pad /= 2) >= 1) {
    //             for (n = 0; n < P / (4 * i); n += pad) {
    //                 for (m = 0; m < pad; m++) {
    //                     for (k = 0; k < Q; k++) {
    //                         if ((A[m + 2*n + j * (P / i)][k] > A[m + 2*n + pad + j * (P / i)][k] && A[m + 2*n + j * (P / i)][Q] == MIN) || (A[m + 2*n + j * (P / i)][k] < A[m + 2*n + pad + j * (P / i)][k] && A[m + 2*n + j * (P / i)][Q] == MAX)) {
    //                             bitonic_swap(A[m + 2*n + j * (P / i)], A[m + 2*n + pad + j * (P / i)], k);
    //                         } // PREPEI NA PROSTETHOUN OROI
    //                     }
    //                 }
    //                 for (m = P / (2 * i); m < P / (2 * i) + pad; m++) {
    //                     for (k = 0; k < Q; k++) {
    //                         if ((A[m + 2*n + j * (P / i)][k] > A[m + 2*n + pad + j * (P / i)][k] && A[m + 2*n + j * (P / i)][Q] == MIN) || (A[m + 2*n + j * (P / i)][k] < A[m + 2*n + pad + j * (P / i)][k] && A[m + 2*n + j * (P / i)][Q] == MAX)) {
    //                             bitonic_swap(A[m + 2*n + j * (P / i)], A[m + 2*n + pad + j * (P / i)], k);
    //                         } // PREPEI NA PROSTETHOUN OROI
    //                     }
    //                 }
    //             }
    //         }

    //         for (s = 0; s < P / i; s += 2) {
    //             if (A[s + 1][Q] == MAX) {
    //                 qsort(A[s + j * (P / i)], Q, sizeof(int), asc_compare);
    //                 qsort(A[s + j * (P / i) + 1], Q, sizeof(int), asc_compare);
    //             } else {
    //                 qsort(A[s + j * (P / i)], Q, sizeof(int), desc_compare);
    //                 qsort(A[s + j * (P / i) + 1], Q, sizeof(int), desc_compare);
    //             }
    //         }

    //         for (int x = 0; x < P; x++) {
    //             for (int y = 0; y < Q; y++) {
    //                 printf("%d ", A[x][y]);
    //             }
    //             printf("\n");
    //         }
    //         printf("\n");
    //     }
    // }

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
