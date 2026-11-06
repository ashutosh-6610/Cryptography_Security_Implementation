# RC4 Benchmarks

**Processor:** AMD Ryzen 5 5000U Series  
**Compiler:** GCC 6.3.0 (MinGW)  
**Operating System:** Windows  
**Key Size:** 5 bytes (40 bits)  
**State Array Size:** 256 bytes  
**Iterations:** 1,000,000  

## Results

### Key Scheduling Algorithm (KSA)

| Optimization | Average Cycles/Iteration |
|---|---:|
| `-O0` | 3894.87 |
| `-O2` | 2293.35 |
| `-O3` | 1443.00 |

### Pseudo-Random Generation Algorithm (PRGA)

| Optimization | Cycles/Byte |
|---|---:|
| `-O0` | 20.4763 |
| `-O2` | 9.4948 |
| `-O3` | 10.2589 |

## Performance Analysis

Compiler optimization improves the performance of both RC4 operations.

For KSA, `-O3` achieves the lowest measured cycle count, reducing the average cycles per initialization by approximately 62.95% compared with `-O0`.

For PRGA, `-O2` achieves the lowest measured cycles per generated keystream byte. The `-O3` configuration shows a slightly higher cycle count than `-O2`.

## Conclusion

The benchmark evaluates the performance of RC4's Key Scheduling Algorithm and Pseudo-Random Generation Algorithm under different compiler optimization levels.

The results demonstrate that compiler optimization can significantly reduce computational overhead, although the highest optimization level does not necessarily provide the best performance for every operation.
