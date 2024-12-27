#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define P 128
#define Q 80
#define REPS 8 // log(P) + 1
#define TOTAL_REPS ((REPS - 1) * REPS / 2)

#define MAX_INTEGER 1000
#define MIN 0
#define MAX 1
#define ASCENT 0
#define DESCENT 1

typedef struct {
    char sort;
    char flow; // Sends (MIN) or receives (MAX)
    char target_pid;
} Instruction;

int ipow(int base, int exp);
int asc_compare(const void *a, const void *b);
int desc_compare(const void *a, const void *b);
void bitonic_swap(int *v1, int *v2, int idx);

int main(int argc, char *argv[]) {

    int i, j, k;
    int uno_reverse, repetition, step, position;
    char ascento[REPS][P];
    int used[P];
    int A[P][Q];
    Instruction instructions[TOTAL_REPS][P];


    srand(time(0));

    // Initialize the array to be sorted with random integers
    for (i = 0; i < P; i++) {
        for (j = 0; j < Q; j++) {
            A[i][j] = rand() % MAX_INTEGER + 1;
        }
    }

    // Print the array
    // for (i = 0; i < P; i++) {
    //     for (j = 0; j < Q; j++) {
    //         printf("%d ", A[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    // Constract the "truth" table for the directions (ascending - descending)
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

    // Using the above create the Instructions "truth" table
    position = 0; // Position
    for (repetition = 0; repetition < REPS - 1; repetition++) { // We have REPS "main" repetitions that involve sorting and TOTAL_REPS
        step = ipow(2, repetition);                         // that involve all layers of merging
        for (i = 0; i < P; i++) {
            // We split the elbow merging into two parts
            // This is the first part - merging of the two elbows
            if ((!ascento[repetition][i] && !ascento[repetition + 1][i]) || (ascento[repetition][i] && ascento[repetition + 1][i])) {
                instructions[position][i].flow = MIN;
                if (!ascento[repetition][i]) {
                    instructions[position][i].target_pid = i + step;
                } else {
                    instructions[position][i].target_pid = i - step;
                }
                instructions[position][i].sort = ascento[repetition][i];
            } else if ((!ascento[repetition][i] && ascento[repetition + 1][i]) || (ascento[repetition][i] && !ascento[repetition + 1][i])) {
                instructions[position][i].flow = MAX;
                if (!ascento[repetition][i]) {
                    instructions[position][i].target_pid = i + step;
                } else {
                    instructions[position][i].target_pid = i - step;
                }
                instructions[position][i].sort = ascento[repetition][i];
            }
        }
        position++;

        // This the second part - "inside" merging of the each part of the elbow
        for (j = 0; j < repetition; j++) {
            step = ipow(2, repetition - j) / 2;
            for (i = 0; i < P; i++) {
                used[i] = 0;
            }
            for (i = 0; i < P; i++) {
                if (!used[i]) {
                    if (!ascento[repetition + 1][i]) {
                        instructions[position][i].flow = MIN;
                        instructions[position][i].target_pid = i + step;
                        instructions[position][i + step].flow = MAX;
                        instructions[position][i + step].target_pid = i;
                        instructions[position][i].sort = 2;
                        instructions[position][i + step].sort = 2;
                    } else {
                        instructions[position][i].flow = MAX;
                        instructions[position][i].target_pid = i + step;
                        instructions[position][i + step].flow = MIN;
                        instructions[position][i + step].target_pid = i;
                        instructions[position][i].sort = 2;
                        instructions[position][i + step].sort = 2;
                    }
                    used[i] = 1;
                    used[i + step] = 1;
                }
            }
            position++;
        }
    }

    // This part is the sorting that will be implemented in the MPI...
    for (repetition = 0; repetition < TOTAL_REPS; repetition++) {
        for (i = 0; i < P; i++) {
            used[i] = 0;
        }
        for (i = 0; i < P; i++) {
            if (!used[i]) {
                // Lathos epeidh to ascento paei me ta vasika vimata
                if (instructions[repetition][i].sort == ASCENT) { // If one must be sorted then both should...
                    qsort(A[i], Q, sizeof(int), asc_compare);
                    qsort(A[instructions[repetition][i].target_pid], Q, sizeof(int), desc_compare);
                } else if (instructions[repetition][i].sort == DESCENT) {
                    qsort(A[i], Q, sizeof(int), desc_compare);
                    qsort(A[instructions[repetition][i].target_pid], Q, sizeof(int), asc_compare);
                }

                if (!instructions[repetition][i].flow) {
                    for (k = 0; k < Q; k++) {
                        if (A[i][k] > A[instructions[repetition][i].target_pid][k]) {
                            bitonic_swap(A[i], A[instructions[repetition][i].target_pid], k);
                        }
                    }
                } else {
                    for (k = 0; k < Q; k++) {
                        if (A[i][k] < A[instructions[repetition][i].target_pid][k]) {
                            bitonic_swap(A[i], A[instructions[repetition][i].target_pid], k);
                        }
                    }
                }
                used[i] = 1;
                used[instructions[repetition][i].target_pid] = 1;
            }
        }
    }

    // Sort the final array
    for (i = 0; i < P; i++) {
        qsort(A[i], Q, sizeof(int), asc_compare);
    }

    // Print the directions "truth" table
    // for (i = 0; i < REPS; i++) {
    //     for (j = 0; j < P; j++) {
    //         printf("%d ", ascento[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    // Print the Instructions "truth" table
    // for (j = 0; j < TOTAL_REPS; j++) {
    //     for (i = 0; i < P; i++) {
    //         if (!instructions[j][i].flow) {
    //             printf(" %d sends to %d | ", i, instructions[j][i].target_pid);
    //         } else {
    //             printf(" %d receives from %d | ", i, instructions[j][i].target_pid);
    //         }
    //     }
    //     printf("\n\n");
    // }

    // Print the sorted array
    for (i = 0; i < P; i++) {
        for (j = 0; j < Q; j++) {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }
    printf("\n");

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
