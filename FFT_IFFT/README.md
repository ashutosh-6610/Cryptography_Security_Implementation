# Performance Analysis

## 1. Experimental Environment

The FFT and IFFT implementations were benchmarked on the following system:

| Parameter | Specification |
|---|---|
| Processor | AMD Ryzen 5 5500U with Radeon Graphics |
| CPU cores | 6 |
| Logical processors | 12 |
| Architecture | AMD64 |
| Operating System | Windows |
| Compiler | MinGW GCC 6.3.0 |

Three compiler optimization configurations were evaluated:

| Configuration | Compiler flags |
|---|---|
| O0 | `-O0` |
| O2 | `-O2` |
| O3 | `-O3 -march=native` |

## 2. Benchmarking Methodology

The performance of FFT and IFFT was evaluated independently for different input sizes.

**Input sizes tested:**

`64, 128, 256, 512, 1024, 2048, 4096, 8192`

The benchmarking procedure involved:

1. Generating input arrays containing complex numbers.
2. Executing the FFT and IFFT operations independently.
3. Performing seven measurement batches for each operation.
4. Calculating the median execution time per call.
5. Recording timestamp-counter ticks and elapsed execution time.

Two timing mechanisms were used:

- **RDTSC/RDTSCP:** Measures timestamp-counter ticks.
- **QueryPerformanceCounter:** Measures elapsed execution time.

The primary performance comparison uses elapsed execution time in microseconds. Timestamp-counter ticks are supplementary measurements and should not be interpreted directly as actual CPU core cycles.

## 3. FFT Performance Results

Execution time in microseconds.

| Input Size | O0 | O2 | O3 |
|---:|---:|---:|---:|
| 64 | 23.211 | 20.906 | 20.277 |
| 128 | 48.844 | 45.431 | 43.674 |
| 256 | 102.913 | 90.833 | 87.633 |
| 512 | 220.908 | 191.485 | 212.356 |
| 1024 | 444.865 | 389.315 | 374.990 |
| 2048 | 925.840 | 809.340 | 774.105 |
| 4096 | 1923.710 | 1665.520 | 1597.940 |
| 8192 | 3996.580 | 3404.165 | 3294.675 |

## 4. IFFT Performance Results

Execution time in microseconds.

| Input Size | O0 | O2 | O3 |
|---:|---:|---:|---:|
| 64 | 23.448 | 21.823 | 20.415 |
| 128 | 49.488 | 44.692 | 42.631 |
| 256 | 103.741 | 92.551 | 88.335 |
| 512 | 216.156 | 190.697 | 186.582 |
| 1024 | 450.070 | 397.355 | 377.150 |
| 2048 | 936.445 | 837.835 | 778.335 |
| 4096 | 1944.525 | 1683.685 | 1601.810 |
| 8192 | 4031.380 | 3468.865 | 3308.070 |

## 5. Comparative Analysis

The geometric mean speedup across all tested input sizes is:

| Comparison | FFT | IFFT |
|---|---:|---:|
| O2 over O0 | 1.136x | 1.125x |
| O3 over O2 | 1.020x | 1.052x |
| O3 over O0 | 1.158x | 1.184x |

### Observations

1. Compiler optimization improves the execution performance of both FFT and IFFT.

2. The O2 configuration consistently outperforms O0 across all tested input sizes.

3. O3 generally provides the best performance, although individual measurements can vary. For example, FFT at input size 512 performs better under O2 than O3.

4. FFT and IFFT have comparable execution times because they use similar recursive decomposition and butterfly operations.

5. Execution time increases with input size, following the expected growth pattern of the FFT algorithm.

## 6. Time Complexity Analysis

The implementation uses the Cooley–Tukey Radix-2 divide-and-conquer algorithm.

The recurrence relation is:

\[
T(n)=2T(n/2)+O(n)
\]

Using the Master Theorem:

\[
\boxed{T(n)=O(n\log_2 n)}
\]

The auxiliary space complexity is:

\[
\boxed{S(n)=O(n)}
\]

The experimental results are consistent with the theoretical time complexity. For example, doubling the input size from 4096 to 8192 under O2 increases FFT execution time from 1665.520 μs to 3404.165 μs, approximately 2.04 times.

## 7. Conclusion

The experimental evaluation demonstrates the computational efficiency of the Cooley–Tukey Radix-2 FFT and its inverse.

The implementation was tested across eight input sizes and three compiler optimization configurations.

The results show that compiler optimization significantly improves execution performance. Among the tested configurations, O3 generally provides the lowest execution time, followed by O2 and O0.

The observed execution-time growth is consistent with the theoretical \(O(n\log n)\) complexity of the FFT algorithm.

Overall, the experiment demonstrates both the correctness and computational performance of the recursive FFT and IFFT implementation.

