#include <stdio.h>
#include <stdlib.h>

#define MAX_INTEGER 100

void reverse(int arr[], int start, int end);
void rotate_left(int arr[], int n, int k);
void rotate_right(int arr[], int n, int k);
void reform_bitonic(int arr[], int n);

int main (int argc, char *argv[]) {

    int i, j;
    int array[8] = {26, 25, 25, 14, 25, 25, 25, 27};
    // int array[8] = {26, 25, 25, 25, 25, 25, 25, 27};
    // int array[8] = {5, 1, 1, 3, 3, 6, 7, 8};

    reform_bitonic(array, 8);

    for (i = 0; i < 8; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    return 0;
}

void reform_bitonic(int arr[], int n) {

    int k, steep;

    steep = arr[0];
    k = 1;
    while (k < n - 1) {
        if (arr[k] < steep && arr[k] < arr[k + 1]) {
            rotate_left(arr, n, k);
            break;
        } else if (arr[k] < steep && arr[k] == arr[k + 1]) {
            k++;
        } else {
            steep = arr[k];
            k++;
        }
    }
}

void reverse(int arr[], int start, int end) {

    while (start < end) {
        int temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}

// Function to perform left rotation of the array by k positions
void rotate_left(int arr[], int n, int k) {

    reverse(arr, 0, k - 1);    // Reverse the first part
    reverse(arr, k, n - 1);    // Reverse the second part
    reverse(arr, 0, n - 1);    // Reverse the whole array
}

void rotate_right(int arr[], int n, int k) {
    
    reverse(arr, 0, n - k - 1);  // Reverse the first part
    reverse(arr, n - k, n - 1);  // Reverse the second part
    reverse(arr, 0, n - 1);      // Reverse the whole array
}
