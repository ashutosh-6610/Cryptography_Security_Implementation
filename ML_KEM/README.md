# Experimental Results and Performance Analysis

## 1. Functional Testing

The implementation was compiled using GCC and tested across all three ML-KEM parameter sets.

**Test command:**
```bash
make test
```

All included tests passed successfully.

| Test | Result |
|---|---|
| SHA3-256 and SHA3-512 test vectors | PASS |
| SHAKE128 and SHAKE256 test vectors | PASS |
| NTT multiplication vs. schoolbook multiplication | PASS |
| NTT inverse round-trip | PASS |
| ByteEncode/ByteDecode round-trip | PASS |
| Compress/Decompress error bound | PASS |
| ML-KEM-512 key generation and KEM round-trip (100 iterations) | PASS |
| ML-KEM-768 key generation and KEM round-trip (100 iterations) | PASS |
| ML-KEM-1024 key generation and KEM round-trip (100 iterations) | PASS |
| Tampered ciphertext implicit rejection | PASS |
| Malformed public-key rejection | PASS |

**Overall result:** All included functional tests passed.

## 2. ML-KEM Demonstration

The ML-KEM-768 demonstration successfully completed the Key Generation, Encapsulation, and Decapsulation workflow.

```text
ML-KEM-768  ek=1184 dk=2400 ct=1088 bytes

sender   K = e785e5bb7048148d34f9206abed37ba16fbbb6c518054d2ca55a5db849940a2c

receiver K = e785e5bb7048148d34f9206abed37ba16fbbb6c518054d2ca55a5db849940a2c

MATCH
```

The sender and receiver independently derived identical 256-bit shared secrets.

## 3. CPU Cycle Benchmark

**Configuration:**
- Compiler: GCC
- Optimization: `-O3 -march=native`
- Warm-up iterations: 200
- Measured runs: 5,001
- Measurement: x86 timestamp counter (TSC)
- Reported statistics: Median and minimum

### Median TSC Ticks per Operation

| Parameter Set | KeyGen | Encaps | Decaps |
|---|---:|---:|---:|
| ML-KEM-512 | 140,217 | 143,304 | 181,629 |
| ML-KEM-768 | 220,269 | 224,280 | 276,087 |
| ML-KEM-1024 | 329,847 | 335,097 | 402,318 |

*Note: TSC ticks are approximate cycle measurements and should not be interpreted as exact core-clock cycles.*

## 4. Wall-Clock Performance Benchmark

**Configuration:**
- Compiler: GCC
- Optimization: `-O3 -march=native`
- Timer: `CLOCK_MONOTONIC`
- Samples: 201
- Reported statistics: Median and minimum execution time

### Median Execution Time

| Parameter Set | KeyGen (μs) | Encaps (μs) | Decaps (μs) |
|---|---:|---:|---:|
| ML-KEM-512 | 67.81 | 69.88 | 87.80 |
| ML-KEM-768 | 107.11 | 109.37 | 133.71 |
| ML-KEM-1024 | 133.56 | 134.65 | 162.70 |

### Throughput

| Parameter Set | KeyGen (ops/s) | Encaps (ops/s) | Decaps (ops/s) |
|---|---:|---:|---:|
| ML-KEM-512 | 14,747 | 14,310 | 11,389 |
| ML-KEM-768 | 9,337 | 9,144 | 7,479 |
| ML-KEM-1024 | 7,488 | 7,426 | 6,146 |

## 5. Component-Level Benchmark

Individual mathematical and cryptographic components were benchmarked to examine their computational costs.

| Component | Median Execution Time (μs) |
|---|---:|
| NTT | 1.83 |
| Inverse NTT | 1.84 |
| MultiplyNTTs | 0.14 |
| SampleNTT | 3.36 |
| SamplePolyCBD (η=2) | 1.39 |
| PRF (SHAKE256) | 0.94 |
| ByteEncode (d=12) | 3.38 |
| Polynomial compression (d=10) | 0.06 |
| SHA3-256 (1,184 bytes) | 8.01 |
| SHA3-512 (64 bytes) | 0.91 |

## 6. Performance Observations

- ML-KEM-512 exhibited the lowest execution time across KeyGen, Encaps, and Decaps.
- ML-KEM-1024 exhibited the highest computational cost among the three parameter sets.
- Decapsulation was the most computationally expensive operation for all three parameter sets.
- Component-level measurements provide insight into the computational costs of polynomial operations, sampling, encoding, and hashing.

## 7. Experimental Environment

| Parameter | Configuration |
|---|---|
| Operating System | Ubuntu on WSL2 |
| Compiler | GCC 15.2.0 |
| Build Utility | GNU Make 4.4.1 |
| Compiler Standard | C11 |
| Optimization | `-O3 -march=native` |
| Cycle Measurement | x86 TSC |
| Wall-Clock Measurement | `CLOCK_MONOTONIC` |

*Performance results are specific to the tested hardware, compiler, optimization settings, and execution environment.*

## 8. Conclusion

The functional tests and demonstration confirm successful execution of the implemented ML-KEM workflows for all three parameter sets. Performance experiments provide comparative measurements of key generation, encapsulation, decapsulation, and individual cryptographic components.

These experiments illustrate the computational performance differences between ML-KEM-512, ML-KEM-768, and ML-KEM-1024.

---

**Acknowledgement:** Credit the original implementation source and its author here, in accordance with the original repository's licence and permission requirements. Distinguish original implementation work from independently conducted testing, benchmarking, and modifications.