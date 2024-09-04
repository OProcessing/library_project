#include <stdio.h>

#define SIZE 5

void averageFilter(const int input[], int output[], int size) {
    int sum = 0;
    for (int i = 0; i < SIZE; i++) {
        sum += input[i];
    }
    int average = sum / SIZE;
    for (int i = 0; i < size; i++) {
        output[i] = average;
    }
}

void movingAverageFilter(const int input[], int output[], int size) {
    int sum = 0;
    for (int i = 0; i < SIZE; ++i) {
        sum += input[i];
    }
    output[0] = sum / SIZE;
    for (int i = 1; i < size - SIZE + 1; ++i) {
        sum = sum - input[i - 1] + input[i + SIZE - 1];
        output[i] = sum / SIZE;
    }
}

int main() {
    int input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int output[SIZE];

    averageFilter(input, output, 5);

    printf("Filtered output: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", output[i]);
    }
    printf("\n");
    
    movingAverageFilter(input, output, 9);

    printf("Filtered output: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", output[i]);
    }
    printf("\n");
    
    return 0;
}
