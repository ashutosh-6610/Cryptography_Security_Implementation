#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <windows.h>



#define PI 3.14159265358979323846
void ifft_helper(double complex input[],double complex output[], int n) {
    complex double odd[n/2], even[n/2], even_ifft[n/2], odd_ifft[n/2];
    int k;
    if (n == 1) {
        output[0] = input[0];
        return;
    }
    for ( k = 0; k < n/2; k++) {
        even[k] = input[2 * k];
        odd[k] = input[2 * k + 1];
    }
    ifft_helper(even,even_ifft,n/2);
    ifft_helper(odd,odd_ifft,n/2);

    double complex omega = cexp(-2.0 * I * PI / n);
    double complex w = 1.0;

    for (k = 0; k < n/2; k++) {
        odd_ifft[k] *= w;
        w *= omega;
    }

    for (k = 0; k < n/2; k++) {
        output[k] = even_ifft[k] + odd_ifft[k];
        output[k + n/2] = even_ifft[k] - odd_ifft[k];
    }
}
void ifft(double complex input[],double complex output[], int n) {
    ifft_helper(input, output,n);
    for (int k = 0; k < n; k++) {
        output[k] /= n;
    }

}
// We assume n to be a power of 2.
void fft(double complex input[],double complex output[], int n) {
    complex double odd[n/2], even[n/2], even_fft[n/2], odd_fft[n/2];
    int k;
    if (n == 1) {
        output[0] = input[0];
        return;
    }
    for ( k = 0; k < n/2; k++) {
        even[k] = input[2 * k];
        odd[k] = input[2 * k + 1];
    }
    fft(even,even_fft,n/2);
    fft(odd,odd_fft,n/2);

    double complex omega = cexp(2.0 * I * PI / n);
    double complex w = 1.0;

    for (k = 0; k < n/2; k++) {
        odd_fft[k] *= w;
        w *= omega;
    }

    for (k = 0; k < n/2; k++) {
        output[k] = even_fft[k] + odd_fft[k];
        output[k + n/2] = even_fft[k] - odd_fft[k];
    }
}

// Read timestamp counter before measurement
static inline unsigned long long read_tsc_start(void)
{
    unsigned int lo, hi;

    __asm__ __volatile__(
        "lfence\n\t"
        "rdtsc"
        : "=a"(lo), "=d"(hi)
        :
        : "memory"
    );

    return ((unsigned long long)hi << 32) | lo;
}

// Read timestamp counter after measurement
static inline unsigned long long read_tsc_end(void)
{
    unsigned int lo, hi, aux;

    __asm__ __volatile__(
        "rdtscp\n\t"
        "lfence"
        : "=a"(lo), "=d"(hi), "=c"(aux)
        :
        : "memory"
    );

    return ((unsigned long long)hi << 32) | lo;
}

#define SAMPLES 7

volatile double benchmark_sink = 0.0;

void sort_values(double values[], int count)
{
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (values[j] > values[j + 1]) {
                double temp = values[j];
                values[j] = values[j + 1];
                values[j + 1] = temp;
            }
        }
    }
}

void benchmark_fft(int n, double complex input[],
                   double complex output[], int iterations)
{
    double tsc_samples[SAMPLES];
    double time_samples[SAMPLES];

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);

    for (int s = 0; s < SAMPLES; s++) {

        LARGE_INTEGER start_time, end_time;

        QueryPerformanceCounter(&start_time);
        unsigned long long start = read_tsc_start();

        for (int i = 0; i < iterations; i++) {
            fft(input, output, n);
        }

        unsigned long long end = read_tsc_end();
        QueryPerformanceCounter(&end_time);

        double sum = 0.0;

        for (int i = 0; i < n; i++) {
            sum += creal(output[i]) + cimag(output[i]);
        }

        benchmark_sink += sum;

        tsc_samples[s] =
            (double)(end - start) / iterations;

        time_samples[s] =
            ((double)(end_time.QuadPart - start_time.QuadPart)
             / frequency.QuadPart) * 1000000.0 / iterations;
    }

    sort_values(tsc_samples, SAMPLES);
    sort_values(time_samples, SAMPLES);

    printf("FFT  | n=%5d | TSC ticks/call=%12.2f | Time=%10.3f us\n",
           n, tsc_samples[SAMPLES / 2],
           time_samples[SAMPLES / 2]);
}

void benchmark_ifft(int n, double complex input[],
                    double complex output[], int iterations)
{
    double tsc_samples[SAMPLES];
    double time_samples[SAMPLES];

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);

    for (int s = 0; s < SAMPLES; s++) {

        LARGE_INTEGER start_time, end_time;

        QueryPerformanceCounter(&start_time);
        unsigned long long start = read_tsc_start();

        for (int i = 0; i < iterations; i++) {
            ifft(input, output, n);
        }

        unsigned long long end = read_tsc_end();
        QueryPerformanceCounter(&end_time);

        double sum = 0.0;

        for (int i = 0; i < n; i++) {
            sum += creal(output[i]) + cimag(output[i]);
        }

        benchmark_sink += sum;

        tsc_samples[s] =
            (double)(end - start) / iterations;

        time_samples[s] =
            ((double)(end_time.QuadPart - start_time.QuadPart)
             / frequency.QuadPart) * 1000000.0 / iterations;
    }

    sort_values(tsc_samples, SAMPLES);
    sort_values(time_samples, SAMPLES);

    printf("IFFT | n=%5d | TSC ticks/call=%12.2f | Time=%10.3f us\n",
           n, tsc_samples[SAMPLES / 2],
           time_samples[SAMPLES / 2]);
}

int main()
{
    int sizes[] = {
        64, 128, 256, 512,
        1024, 2048, 4096, 8192
    };

    int number_of_sizes = sizeof(sizes) / sizeof(sizes[0]);

    printf("\nFFT/IFFT PERFORMANCE BENCHMARK\n");
    printf("============================================\n");

    for (int j = 0; j < number_of_sizes; j++) {

        int n = sizes[j];

        double complex *input =
            calloc(n, sizeof(double complex));

        double complex *frequency =
            calloc(n, sizeof(double complex));

        double complex *output =
            calloc(n, sizeof(double complex));

        if (!input || !frequency || !output) {
            printf("Memory allocation failed for n=%d\n", n);
            free(input);
            free(frequency);
            free(output);
            return 1;
        }

        for (int i = 0; i < n; i++) {
            input[i] = (double)(i % 100 + 1);
        }

        // Prepare frequency-domain input for IFFT
        fft(input, frequency, n);

        // Choose enough repetitions for stable measurements
        int iterations = 20000 / n;

        if (iterations < 20)
            iterations = 20;

        printf("\nInput size: %d | Repetitions: %d\n",
               n, iterations);

        benchmark_fft(n, input, output, iterations);

        benchmark_ifft(n, frequency, output, iterations);

        free(input);
        free(frequency);
        free(output);
    }

    printf("\nBenchmark checksum: %.4f\n", benchmark_sink);

    return 0;
}
