#include "../include/vectoral_bitonic_sort.h"

int mpi_bitonic_warmup(int p, int reps, int total_reps) {

    int i, j, k;
    int uno_reverse, repetition, step, position;
    char ascento[reps][p];
    int used[p];
    Instruction instructions[total_reps][p];

    // Constract the "truth" table for the directions (ascending - descending)
    repetition = 0;
    for (i = 1; i <= p; i *= 2) {
        uno_reverse = 0;
        for (j = 0; j < p; j += i) {
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
    for (repetition = 0; repetition < reps - 1; repetition++) { // We have reps "main" repetitions that involve sorting and total_reps
        step = ipow(2, repetition);                         // that involve all layers of merging
        for (i = 0; i < p; i++) {
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
            for (i = 0; i < p; i++) {
                used[i] = 0;
            }
            for (i = 0; i < p; i++) {
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
