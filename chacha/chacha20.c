#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>

#include <x86intrin.h>

#define ROTL32(x, n) (((x) << (n)) | ((x) >> (32 - (n))))


void quarter_round(uint32_t *state, int a, int b, int c, int d) {
    state[a] += state[b]; state[d] ^= state[a]; state[d] = ROTL32(state[d], 16);
    state[c] += state[d]; state[b] ^= state[c]; state[b] = ROTL32(state[b], 12);
    state[a] += state[b]; state[d] ^= state[a]; state[d] = ROTL32(state[d], 8);
    state[c] += state[d]; state[b] ^= state[c]; state[b] = ROTL32(state[b], 7);
}

void column_round(uint32_t *state) {
    quarter_round(state, 0, 4, 8, 12);
    quarter_round(state, 1, 5, 9, 13);
    quarter_round(state, 2, 6, 10, 14);
    quarter_round(state, 3, 7, 11, 15);
}

void diagonal_round(uint32_t *state) {
    quarter_round(state, 0, 5, 10, 15);
    quarter_round(state, 1, 6, 11, 12);
    quarter_round(state, 2, 7, 8, 13);
    quarter_round(state, 3, 4, 9, 14);
}

void permute(uint32_t *state) {
	int i;
    for (i = 0; i < 10; i++) {
        column_round(state);
        diagonal_round(state);
    }
}

void increment_counter(uint32_t *state) {
    if (++state[12] == 0) {
        ++state[13];
    }
}

void chacha20_init(uint32_t *state, const uint8_t *key, const uint8_t *nonce, uint64_t counter) {
	int i;
    state[0] = 0x61707865;
    state[1] = 0x3320646E;
    state[2] = 0x79622D32;
    state[3] = 0x6B206574;

    for (i = 0; i < 8; i++) {
        state[4 + i] = ((uint32_t)key[i*4]) | 
                       (((uint32_t)key[i*4 + 1]) << 8) | 
                       (((uint32_t)key[i*4 + 2]) << 16) | 
                       (((uint32_t)key[i*4 + 3]) << 24);
    }

    state[12] = (uint32_t)(counter & 0xFFFFFFFF);
    state[13] = (uint32_t)(counter >> 32);

    state[14] = ((uint32_t)nonce[0]) | (((uint32_t)nonce[1]) << 8) | 
                (((uint32_t)nonce[2]) << 16) | (((uint32_t)nonce[3]) << 24);
                
    state[15] = ((uint32_t)nonce[4]) | (((uint32_t)nonce[5]) << 8) | 
                (((uint32_t)nonce[6]) << 16) | (((uint32_t)nonce[7]) << 24);
}

void chacha20_encrypt(uint32_t *initial_state, const uint8_t *in, uint8_t *out, size_t len) {
    uint32_t state[16];
    uint8_t keystream[64];
    size_t i, j;

    for (i = 0; i < len; i += 64) {
        memcpy(state, initial_state, sizeof(state));
        permute(state);

        for (j = 0; j < 16; j++) {
            state[j] += initial_state[j];
        }

        for (j = 0; j < 16; j++) {
            keystream[j*4]     = (uint8_t)(state[j]);
            keystream[j*4 + 1] = (uint8_t)(state[j] >> 8);
            keystream[j*4 + 2] = (uint8_t)(state[j] >> 16);
            keystream[j*4 + 3] = (uint8_t)(state[j] >> 24);
        }

        size_t block_len = (len - i) < 64 ? (len - i) : 64;
        for (j = 0; j < block_len; j++) {
            out[i + j] = in[i + j] ^ keystream[j];
        }

        increment_counter(initial_state);
    }
}


int main(void)
{
    uint8_t key[32] = {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
        0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
        0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
        0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38
    };

    uint8_t nonce[8] = {
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x01
    };

    uint8_t plaintext[64];
    uint8_t ciphertext[64];
    uint32_t state[16];

    const int ITERATIONS = 1000000;
    const int MESSAGE_SIZE = 64;

    uint64_t start_cycles, end_cycles;
    uint64_t total_cycles;

    LARGE_INTEGER frequency, start_time, end_time;

    volatile uint32_t checksum = 0;

    size_t i;
    int iter;

    /* Generate a fixed 64-byte test message */
    for (i = 0; i < MESSAGE_SIZE; i++)
    {
        plaintext[i] = (uint8_t)i;
    }

    /* Initialize ChaCha20 */
    chacha20_init(state, key, nonce, 1);

    /* Get high-resolution timer frequency */
    QueryPerformanceFrequency(&frequency);

    /* Start time measurement */
    QueryPerformanceCounter(&start_time);

    /* Start CPU cycle measurement */
    _mm_lfence();
    start_cycles = __rdtsc();

    /* Perform 1,000,000 encryption iterations */
    for (iter = 0; iter < ITERATIONS; iter++)
    {
        chacha20_encrypt(
            state,
            plaintext,
            ciphertext,
            MESSAGE_SIZE
        );
    }

    /* End CPU cycle measurement */
    _mm_lfence();
    end_cycles = __rdtsc();

    /* End time measurement */
    QueryPerformanceCounter(&end_time);

    total_cycles = end_cycles - start_cycles;

    double total_time_ns =
        ((double)(end_time.QuadPart - start_time.QuadPart)
        * 1000000000.0) / frequency.QuadPart;

    double cycles_per_iteration =
        (double)total_cycles / ITERATIONS;

    double cycles_per_byte =
        (double)total_cycles / (ITERATIONS * MESSAGE_SIZE);

    double time_per_iteration =
        total_time_ns / ITERATIONS;

    double time_per_byte =
        total_time_ns / (ITERATIONS * MESSAGE_SIZE);

    /* Use the output so the benchmark result remains observable */
    for (i = 0; i < MESSAGE_SIZE; i++)
    {
        checksum += ciphertext[i];
    }

    printf("\n--- CHACHA20 BENCHMARK RESULTS ---\n");

    printf("Message size       : %d bytes\n", MESSAGE_SIZE);
    printf("Iterations         : %d\n", ITERATIONS);

    printf("Total cycles       : %llu\n",
           (unsigned long long)total_cycles);

    printf("Cycles/iteration   : %.2f\n",
           cycles_per_iteration);

    printf("Cycles/byte        : %.4f\n",
           cycles_per_byte);

    printf("Total time         : %.2f ns\n",
           total_time_ns);

    printf("Time/iteration     : %.2f ns\n",
           time_per_iteration);

    printf("Time/byte          : %.4f ns\n",
           time_per_byte);

    printf("Output checksum    : %u\n", (unsigned int)checksum);

    return 0;
}
