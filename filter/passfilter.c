#include <stdio.h>

#define ALPHA_LPF 0.1
#define ALPHA_HPF 0.1

void lowPassFilter(const float input[], float output[], int size) {
    output[0] = input[0];
    for (int i = 1; i < size; ++i) {
        output[i] = ALPHA_LPF * input[i] + (1 - ALPHA_LPF) * output[i - 1];
    }
}

void highPassFilter(const float input[], float output[], int size) {
    float prev_input = input[0];
    float prev_output = input[0];

    for (int i = 1; i < size; ++i) {
        output[i] = ALPHA_HPF * (prev_output + input[i] - prev_input);
        prev_input = input[i];
        prev_output = output[i];
    }
}

void bandPassFilter(const float input[], float output[], int size) {
    float lpf_output[size];
    float hpf_output[size];

    lowPassFilter(input, lpf_output, size);

    highPassFilter(input, hpf_output, size);

    for (int i = 0; i < size; ++i) {
        output[i] = lpf_output[i] - hpf_output[i];
    }
}

int main() {
    float input[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
    float output[9];
    
    bandPassFilter(input, output, 9);

    printf("Filtered output: ");
    for (int i = 0; i < 9; ++i) {
        printf("%.2f ", output[i]);
    }
    printf("\n");
    
    return 0;
}
