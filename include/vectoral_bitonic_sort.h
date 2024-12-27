#ifndef V_BITONIC_SORT_H
#define V_BITONIC_SORT_H

#include <stdlib.h>

#define MIN 0
#define MAX 1
#define ASCENT 0
#define DESCENT 1

typedef struct {
    char sort;
    char flow; // Sends (MIN) or receives (MAX)
    char target_pid;
} Instruction;

Instruction **mpi_bitonic_warmup(int p, int reps, int total_reps);
int ipow(int base, int exp);

#endif // V_BITONIC_SORT_H
