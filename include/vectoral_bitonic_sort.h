#ifndef V_BITONIC_SORT_H
#define V_BITONIC_SORT_H

#define MIN 0
#define MAX 1
#define ASCENT 0
#define DESCENT 1

typedef struct {
    char sort;
    char flow; // Sends (MIN) or receives (MAX)
    char target_pid;
} Instruction;

int mpi_bitonic_warmup();
int ipow(int base, int exp);

#endif // V_BITONIC_SORT_H
