#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define P 8
#define Q 4
#define REPS 4 // log(P) + 1
#define TOTAL_REPS ((REPS - 1) * REPS / 2)

#define MAX_INTEGER 100
#define MIN 0
#define MAX 1
#define ASCENT 0
#define DESCENT 1

typedef struct {
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
    char minmax[TOTAL_REPS][P];
    int used[P];
    int A[P][Q + 1];
    Instruction instructions[TOTAL_REPS][P];


    srand(time(0));

    // Initialize the array to be sorted with random integers
    // for (i = 0; i < P; i++) {
    //     for (j = 0; j < Q; j++) {
    //         A[i][j] = rand() % MAX_INTEGER + 1;
    //     }
    // }

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

    // Using the above create the MIN-MAX and the instructions "truth" table
    position = 0; // Position
    for (repetition = 0; repetition < REPS - 1; repetition++) { // We have REPS "main" repetitions that involve sorting and TOTAL_REPS
        step = ipow(2, repetition);                         // that involve all layers of merging
        for (i = 0; i < P; i++) {
            // We split the elbow merging into two parts
            // This is the first part - merging of the two elbows
            if (!ascento[repetition][i] && !ascento[repetition + 1][i]) {
                minmax[position][i] = MIN;
                instructions[position][i].flow = MIN;
                instructions[position][i].target_pid = i + step;
            } else if (!ascento[repetition][i] && ascento[repetition + 1][i]) {
                minmax[position][i] = MAX;
                instructions[position][i].flow = MAX;
                instructions[position][i].target_pid = i - step;
            } else if (ascento[repetition][i] && !ascento[repetition + 1][i]) {
                minmax[position][i] = MAX;
                instructions[position][i].flow = MAX;
                instructions[position][i].target_pid = i - step;
            } else {
                minmax[position][i] = MIN;
                instructions[position][i].flow = MIN;
                instructions[position][i].target_pid = i + step;
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
                        minmax[position][i] = MIN;
                        minmax[position][i + step] = MAX;
                        instructions[position][i].flow = MIN;
                        instructions[position][i].target_pid = i + step;
                        instructions[position][i + step].flow = MAX;
                        instructions[position][i + step].target_pid = i;
                    } else {
                        minmax[position][i] = MAX;
                        minmax[position][i + step] = MIN;
                        instructions[position][i].flow = MAX;
                        instructions[position][i].target_pid = i + step;
                        instructions[position][i + step].flow = MIN;
                        instructions[position][i + step].target_pid = i;
                    }
                    used[i] = 1;
                    used[i + step] = 1;
                }
            }
            position++;
        }
    }

    // Print the directions "truth" table
    // for (i = 0; i < REPS; i++) {
    //     for (j = 0; j < P; j++) {
    //         printf("%d ", ascento[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    // Print the minmax "truth" table
    // for (i = 0; i < TOTAL_REPS; i++) {
    //     for (j = 0; j < P; j++) {
    //         if (!minmax[i][j]) {
    //             printf("MIN ");
    //         } else {
    //             printf("MAX ");
    //         }
    //     }
    //     printf("\n\n");
    // }

    // Print the sorted array
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
