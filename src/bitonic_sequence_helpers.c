#include "../include/bitonic_sequence_helpers.h"

void reform_bitonic(int *arr, int n) {

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

void reverse(int *arr, int start, int end) {

    while (start < end) {
        int temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}

void rotate_left(int *arr, int n, int k) {

    reverse(arr, 0, k - 1);    // Reverse the first part
    reverse(arr, k, n - 1);    // Reverse the second part
    reverse(arr, 0, n - 1);    // Reverse the whole array
}

void rotate_right(int *arr, int n, int k) {
    
    reverse(arr, 0, n - k - 1);  // Reverse the first part
    reverse(arr, n - k, n - 1);  // Reverse the second part
    reverse(arr, 0, n - 1);      // Reverse the whole array
}

void elbow_sort(int *arr, int n, int flow) {

    int i, t;
    int peak, low, high;
    int *temp;

    // MPOROUME PIO GRIGORA APO O(5*n/2 + n(pisw sto kanoniko array)) AN EXOUME DUPLICATES???
    
    reform_bitonic(arr, n);

    // Here we find the peak of the standard form bitonic
    // by taking advantage of the fact that we always start
    // traversing the not descending part
    peak = -1;
    for (i = 1; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            peak = i;
            break;
        }
    }
    // printf("PEAK: %d %d\n", peak, arr[peak]);
    
    // If no peak was found then the whole sequence
    // is either not ascending or not descending
    if (peak == -1 && arr[0] <= arr[n - 1]) {
        return;
    } else if (peak == -1 && arr[0] > arr[n - 1]) {
        reverse(arr, 0, n - 1);
        return;
    }

    // Temporary array to store the sorted elements
    temp = (int *)malloc(n * sizeof(int));

    // Merging both the sorted arrays before and after the peak element
    low = 0;
    high = n - 1;
    t = 0;
    while (low != high) {
        // Storing the smaller value element in the temp array
        if (arr[low] < arr[high]) {
            temp[t] = arr[low];
            t++;
            low++;
        } else {
            temp[t] = arr[high];
            t++;
            high--;
        }
    }
    temp[t] = arr[peak];

    // Storing all elements of the temporary array back in the original array
    if (!flow) {
        for (i = 0; i < n; i++) {
            arr[i] = temp[i];
        }
    } else {
        for (i = 0; i < n; i++) {
            arr[i] = temp[n - i - 1];
        }
    }

    free(temp);
}
