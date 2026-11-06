#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include <x86intrin.h>

#define ITERATIONS #define ITERATIONS 1000000
typedef uint8_t u8;

static inline void swap(u8 *a, u8 *b) {
  u8 temp = *a;
  *a = *b;
  *b = temp;
}

void key_init( const u8 *key, const size_t key_len, u8 S[256]) {
  for (int i = 0; i < 256; i++) {
    S[i] = i;
  }
  
  u8 j = 0;
  for (int i = 0; i < 256; i++) {
    j += S[i] + key [ i % key_len ]; // this will roundup modulo 256 since variables are u8
    swap(&S[i] , &S[j]);
  }
  
}

u8 keystream(u8 S[256], u8 *i, u8 *j) {
  u8 ii = *i + 1;
  u8 jj = *j + S[ii];
  swap(&S[ii], &S[jj]);
  
  *i =ii; *j =jj;
  
  return S[(u8)(S[ii] + S[jj])];
}

int main() {
  u8 i = 0, j = 0;
  u8 S[256];
  u8 key[] = {0x01,0x02,0x03,0x04,0x05};

  uint64_t start_cycles;
  uint64_t end_cycles;
  uint64_t total_cycles;
  

  volatile u8 checksum = 0;
  unsigned int cpu_id;
  // Measuring Number of cycles required for Key Intialization
  start_cycles = __rdtscp(&cpu_id);
  
  for (uint64_t m = 0; m < ITERATIONS; m++) {
    key_init(key, sizeof(key), S);
    checksum ^= S[0];
  }
  end_cycles = __rdtscp(&cpu_id);
  total_cycles = end_cycles - start_cycles;  
  
  printf("Avg no. of cycles for key init          : %.2f\n", (double)total_cycles / ITERATIONS);

  //Testing PRGA
  start_cycles = __rdtscp(&cpu_id);
  
  for (uint64_t m = 0; m < ITERATIONS; m++) {
    checksum ^= keystream(S, &i, &j);
  }

  end_cycles = __rdtscp(&cpu_id);
  total_cycles = end_cycles - start_cycles;
  
  double cycles_per_byte = (double) total_cycles / ITERATIONS;
  
  printf("Cycles / Byte for key_stream generation : %.4f\n", cycles_per_byte);
  printf("Checksum :%d\n", checksum);
  return 0;
}
