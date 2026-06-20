# Phase 6 - RVV Optimization Results

## Objective

Evaluate the effect of RVV optimization on image-processing kernels.

The study focused on:

1. Gaussian Blur
2. Sobel X
3. Gradient Magnitude (L1)

The primary optimization target was Gaussian Blur because profiling identified it as the largest hotspot.

---

## Profiling Results

| Kernel | Runtime Share |
|----------|----------|
| Gaussian Blur | 39.2% |
| Sobel | 31.6% |

Gaussian Blur was therefore selected as the primary RVV optimization target.

---

## Gaussian RVV Implementation

The RVV implementation introduced:

- Strip mining using vsetvl
- RVV vector accumulators
- LMUL exploration
- VLEN exploration

---

## LMUL Study

| LMUL | Total Time (50 iterations) |
|------|------|
| m1 | 12.919 s |
| m2 | 13.704 s |
| m4 | 11.552 s |

Best configuration: LMUL = m4

Improvement over m1:

12.919 / 11.552 = 1.12x

---

## VLEN Study

| VLEN | Total Time (50 iterations) |
|------|------|
| 128 | 11.552 s |
| 256 | 4.597 s |
| 512 | 2.693 s |

Best configuration: VLEN = 512

Improvement:

11.552 / 2.693 = 4.29x

---

## Scalar vs RVV Gaussian

Scalar Gaussian:

58-64 ms per iteration

Best RVV Gaussian:

LMUL = m4
VLEN = 512

53.9 ms per iteration

Observed speedup:

1.08x - 1.19x

---

## Magnitude Study

Baseline:

0.425 s / 100 iterations

RVV Attempt:

0.928 s / 100 iterations

Result:

RVV implementation increased execution time.

The baseline implementation was retained.

---

## Conclusions

1. RVV strip mining was successfully implemented.
2. LMUL affected performance significantly.
3. Increasing VLEN improved throughput substantially.
4. The best RVV Gaussian implementation slightly outperformed the scalar baseline.
5. Not all RVV transformations improve performance; memory traffic can dominate execution time.
