# Phase 6 – RVV Optimization Summary

## Gaussian Optimization

| Configuration | Runtime (s) | Relative Speedup |
|--------------|------------:|-----------------:|
| VLEN 128 | 9.003 | 1.00× |
| VLEN 256 | 4.462 | 2.02× |
| VLEN 512 | 2.687 | 3.35× |

### LMUL Study

| LMUL | Runtime (s) | Relative Speedup |
|------|------------:|-----------------:|
| m1 | 12.919 | 0.89× |
| m2 | 13.704 | 0.84× |
| m4 | 11.552 | 1.00× |

**Best Configuration:** LMUL = m4, VLEN = 512

---

## Kernel-Level RVV Evaluation

| Kernel | Scalar Runtime (ms) | RVV Runtime (ms) | Result |
|---------|-------------------:|-----------------:|---------|
| Gaussian Blur | 58–64 | 49.2 | RVV Faster |
| Sobel Filter | 13.84 | 36.58 | RVV Slower |
| Magnitude | 4.25 | 2.24 | RVV Faster |

### Derived Metrics

| Metric | Value |
|---------|------:|
| Sobel Slowdown | 2.64× |
| Magnitude Speedup | 1.90× |

---

## Direction Optimization Study

| Implementation | Runtime (s) | Speedup vs Original |
|----------------|------------:|--------------------:|
| Original atan2() | 5.040 | 1.00× |
| Integer Quantized | 0.144 | 35.00× |
| Experimental RVV | 0.171 | 29.47× |

### Direction Comparison

| Comparison | Ratio |
|------------|------:|
| Quantized vs atan2 | 35.00× Faster |
| RVV vs atan2 | 29.47× Faster |
| RVV vs Quantized | 1.19× Slower |

---

## Final Conclusions

1. Gaussian Blur benefited significantly from RVV acceleration.
2. Magnitude computation achieved a measurable RVV speedup.
3. Sobel filtering became slower after RVV vectorization due to overhead and memory access behavior.
4. The largest improvement in the entire project came from Direction optimization.
5. Replacing atan2() with integer quantization reduced execution time from 5.040 s to 0.144 s.
6. The optimized scalar Direction implementation outperformed the experimental RVV implementation.
7. Algorithmic optimization provided larger gains than SIMD acceleration for Direction.
8. RVV performance depends strongly on workload structure and arithmetic intensity.
9. Best Gaussian configuration was LMUL = m4 and VLEN = 512.

---

## Best Results Achieved

| Optimization | Best Result |
|--------------|------------|
| Gaussian RVV | 3.35× Speedup |
| Magnitude RVV | 1.90× Speedup |
| Direction Quantization | 35.00× Speedup |
| Best LMUL | m4 |
| Best VLEN | 512 |

