# ChaCha20 Benchmarks

**Processor:** AMD Ryzen 5 5000U Series  
**Compiler:** GCC 6.3.0 (MinGW)  
**Operating System:** Windows  
**Algorithm:** ChaCha20 (64-bit counter and 64-bit nonce variant)  
**Message size:** 64 bytes  
**Iterations:** 1,000,000  

## Results

| Optimization | Cycles/iteration | Cycles/byte | Time/iteration (ns) | Time/byte (ns) |
|---|---:|---:|---:|---:|
| `-O0` | 3639.99 | 56.8749 | 1736.64 | 27.1350 |
| `-O2` | 972.84 | 15.2006 | 464.15 | 7.2523 |
| `-O3` | 295.46 | 4.6165 | 140.97 | 2.2026 |

## Benchmark Methodology

- A fixed 64-byte plaintext is used.
- Each configuration performs 1,000,000 encryption iterations.
- CPU cycles are measured using the `__rdtsc()` timestamp counter.
- Execution time is measured using the Windows high-resolution performance counter.
- The same key, nonce and message are used across optimization configurations.

## Performance Analysis

Compiler optimization significantly improves ChaCha20 performance.

Compared with `-O0`:

- `-O2` reduces measured CPU cycles by approximately 73.27%.
- `-O3` reduces measured CPU cycles by approximately 91.88%.

The `-O3` configuration achieves the lowest measured cycles per byte and execution time per byte.

## Conclusion

The benchmark demonstrates the impact of compiler optimization on the performance of the ChaCha20 implementation. The results show substantial improvements when moving from `-O0` to `-O2` and `-O3`.

Results are specific to the hardware, compiler version, and system environment used during testing.
