#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fftw3.h>

#define N 1024   // Number of samples
#define FS 1000  // Sampling frequency
#define NUM_TRAIN 10
#define NUM_GUARD 2
#define THRESHOLD_FACTOR 5

void generate_signal(double* signal, double* noise, int num_samples, double* frequencies, int num_freqs, double noise_power);
void ca_cfar(double* signal, int num_cells, int num_train, int num_guard, double threshold_factor, double* threshold, int* detection);



int main() {
    // Initialize variables
    double signal[N], noise[N];
    double frequencies[] = {50, 120, 200};  // Target frequencies (Hz)
    int num_freqs = sizeof(frequencies) / sizeof(frequencies[0]);
    double noise_power = 1.0;

    // Generate signal
    generate_signal(signal, noise, N, frequencies, num_freqs, noise_power);

    // FFT setup
    fftw_complex in[N], out[N];
    fftw_plan plan;
    for (int i = 0; i < N; i++) {
        in[i][0] = signal[i]; // Real part
        in[i][1] = 0.0;       // Imaginary part
    }
    plan = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);

    // Compute magnitude of FFT result
    double fft_magnitude[N];
    for (int i = 0; i < N; i++) {
        fft_magnitude[i] = sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]);
    }

    // Apply CA-CFAR filtering
    double threshold[N] = {0};
    int detection[N] = {0};
    ca_cfar(fft_magnitude, N, NUM_TRAIN, NUM_GUARD, THRESHOLD_FACTOR, threshold, detection);

    // Print results
    printf("Index\tFFT Mag\tThreshold\tDetection\n");
    for (int i = 0; i < N / 2; i++) {
        printf("%d\t%.2f\t%.2f\t%d\n", i, fft_magnitude[i], threshold[i], detection[i]);
    }

    // Clean up
    fftw_destroy_plan(plan);

    return 0;
}

// Function to generate sinusoidal signal with noise
void generate_signal(double* signal, double* noise, int num_samples, double* frequencies, int num_freqs, double noise_power) {
    for (int i = 0; i < num_samples; i++) {
        signal[i] = 0.0;
        for (int j = 0; j < num_freqs; j++) {
            signal[i] += sin(2.0 * M_PI * frequencies[j] * i / FS);
        }
        noise[i] = sqrt(noise_power) * ((double)rand() / RAND_MAX - 0.5);
        signal[i] += noise[i];
    }
}


// CA-CFAR filtering
void ca_cfar(double* signal, int num_cells, int num_train, int num_guard, double threshold_factor, double* threshold, int* detection) {
    for (int i = num_train + num_guard; i < num_cells - num_train - num_guard; i++) {
        double noise_sum = 0.0;
        for (int j = i - num_train - num_guard; j < i - num_guard; j++) {
            noise_sum += signal[j];
        }
        for (int j = i + num_guard + 1; j <= i + num_train + num_guard; j++) {
            noise_sum += signal[j];
        }
        double noise_level = noise_sum / (2.0 * num_train);
        threshold[i] = noise_level * threshold_factor;

        // Detection logic
        if (signal[i] > threshold[i]) {
            detection[i] = 1;
        } else {
            detection[i] = 0;
        }
    }
}
