# AES-256 Encryption and Decryption

## Overview
This project implements the Advanced Encryption Standard (AES) with a 256-bit key (AES-256) in C.

The implementation includes:
- AES-256 Key Expansion
- SubBytes and InvSubBytes
- ShiftRows and InvShiftRows
- MixColumns and InvMixColumns
- AddRoundKey
- Encryption and Decryption
- CPU cycle performance benchmarking using `__rdtsc()`

## Test Configuration

| Parameter | Value |
|---|---|
| Algorithm | AES-256 |
| Key Size | 256 bits (32 bytes) |
| Block Size | 128 bits (16 bytes) |
| Number of Rounds | 14 |
| Benchmark Iterations | 1,000,000 |
| Compiler | GCC |
| Optimization | `-O2 -march=native` |
| Operating System | Windows |

## Test Vector

**AES-256 Key:**
```text
60 3d eb 10 15 ca 71 be 2b 73 ae f0 85 7d 77 81
1f 35 2c 07 3b 61 08 d7 2d 98 10 a3 09 14 df f4
```

**Plaintext:**
```text
6b c1 be e2 2e 40 9f 96 e9 3d 7e 11 73 93 17 2a
```

**Generated Ciphertext:**
```text
f3 ee d1 bd b5 d2 a0 3c 06 4b 5a 7e 3d b1 81 f8
```

**Decrypted Plaintext:**
```text
6b c1 be e2 2e 40 9f 96 e9 3d 7e 11 73 93 17 2a
```

The generated ciphertext matches the NIST AES-256 test vector, and decryption successfully recovers the original plaintext.

## Performance Benchmarking

Performance was measured over 1,000,000 iterations using the `__rdtsc()` CPU timestamp counter.

### Compilation

```bash
gcc -O2 -march=native aes.c -o aes
```

### Execution

```bash
./aes
```

On Windows PowerShell:

```powershell
.\aes
```

### Benchmark Results

| Metric | Encryption | Decryption |
|---|---:|---:|
| Average CPU Cycles per Block | 1012.75 | 1600.61 |
| Cycles Per Byte (CPB) | 63.30 | 100.04 |

### Compilation Without Optimization

For comparison, the program was also compiled without optimization:

```bash
gcc aes.c -o aes
```

| Metric | Encryption | Decryption |
|---|---:|---:|
| Average CPU Cycles per Block | 3737.85 | 5644.80 |
| Cycles Per Byte (CPB) | 233.62 | 352.80 |

Compiler optimization significantly reduced the measured CPU cycle count for both encryption and decryption.

## Conclusion

The AES-256 implementation successfully performs both encryption and decryption. The generated ciphertext matches the NIST test vector, confirming the correctness of the implementation for the tested input.

Performance benchmarking demonstrates the impact of compiler optimization on AES-256 execution time, with `-O2 -march=native` providing substantially better results than compilation without optimization.
