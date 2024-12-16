#include <stdio.h>

#define P 8
#define REPS 4 // log(P) + 1
#define TOTAL_REPS ((REPS - 1) * REPS / 2)

#define MIN 0
#define MAX 1
#define ASCENT 0
#define DESCENT 1

typedef struct {
    char sort; // 0: ascending, 1: descending, 2: do nothing
    char flow; // Sends (MIN) or receives (MAX)
    char target_pid;
} Instruction;

int ipow(int base, int exp);

int main(int argc, char *argv[]) {

    int i, j, k;
    int uno_reverse, repetition, step, position;
    char ascento[REPS][P];
    int used[P];
    Instruction instructions[TOTAL_REPS][P];

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

    // Print the directions "truth" table
    // for (i = 0; i < REPS; i++) {
    //     for (j = 0; j < P; j++) {
    //         printf("%d ", ascento[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    // Print the Instructions "truth" table
    for (j = 0; j < P; j++) {
        printf("For process %d\n", j);
        printf("--------------\n");
        for (i = 0; i < TOTAL_REPS; i++) {
            if (instructions[i][j].sort == ASCENT) {
                printf("sort elements of %d ascending\n", j);
            } else if (instructions[i][j].sort == DESCENT) {
                printf("sort elements of %d descending\n", j);
            }

            if (!instructions[i][j].flow) {
                printf("%d sends to %d\n", j, instructions[i][j].target_pid);
            } else {
                printf("%d receives from %d\n", j, instructions[i][j].target_pid);
            }
        }
        printf("\n\n");
    }

    return 0;
}

int ipow(int base, int exp) {

    int i;
    int result;

    result = 1;
    for (i = 0; i < exp; i++) {
        result *= base;
    }

    return result;
}
