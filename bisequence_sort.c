#include <stdio.h>
#include <stdlib.h>

#define MAX_INTEGER 100

void reverse(int arr[], int start, int end);
void rotate_left(int arr[], int n, int k);
void rotate_right(int arr[], int n, int k);
void reform_bitonic(int arr[], int n);

int main (int argc, char *argv[]) {

    int i, j;
    int array[256] = {100,99,99,99,99,99,98,98,98,97,97,97,96,96,96,95,95,95,95,93,93,92,91,91,90,90,90,89,89,89,88,88,88,87,87,87,86,86,83,83,83,83,82,81,80,80,80,79,79,79,78,78,77,77,77,76,76,76,75,75,75,74,74,74,74,73,72,71,71,71,70,70,69,69,69,69,68,67,67,67,67,65,65,65,64,63,62,62,61,60,60,60,59,59,58,58,58,57,57,57,56,56,56,56,55,55,55,55,54,53,53,53,53,53,52,52,52,52,51,51,50,50,50,50,49,49,50,50,50,50,51,51,52,52,53,53,53,53,54,55,56,57,57,58,58,59,59,59,59,60,60,60,60,60,61,61,62,62,63,63,63,64,65,65,66,67,67,67,67,68,68,68,68,68,68,69,69,69,70,70,70,70,70,71,71,71,72,72,72,73,74,74,75,76,76,76,76,78,79,79,79,80,81,81,82,83,83,83,84,84,84,85,85,86,86,86,86,86,87,87,88,88,88,89,89,89,90,90,90,90,90,90,90,92,92,93,93,93,93,94,94,95,95,95,95,96,96,96,96,96,97,97,98,99,99,99};
    // int array[8] = {26, 25, 25, 14, 25, 25, 25, 27};
    // int array[8] = {26, 25, 25, 25, 25, 25, 25, 27};
    // int array[8] = {5, 1, 1, 3, 3, 6, 7, 8};

    reform_bitonic(array, 256);

    for (i = 0; i < 256; i++) {
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
