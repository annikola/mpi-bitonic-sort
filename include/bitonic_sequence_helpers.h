#ifndef BITONIC_SEQUENCE_HELPERS
#define BITONIC_SEQUENCE_HELPERS

#include <stdlib.h>

void reform_bitonic(int *arr, int n);

void reverse(int *arr, int start, int end);

void rotate_left(int *arr, int n, int k);

void rotate_right(int *arr, int n, int k);

void elbow_sort(int *arr, int n, int flow);

#endif // BITONIC_SEQUENCE_HELPERS
