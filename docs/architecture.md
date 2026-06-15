# RV_Project Architecture

## High-Level Pipeline

Input Image
→ Gaussian Blur
→ Sobel X / Sobel Y
→ Gradient Magnitude
→ Direction Quantization
→ Non-Maximum Suppression
→ Double Threshold
→ Hysteresis
→ Final Edge Map

## Repository Structure

include/
- Public interfaces and headers

src/
- Scalar implementation
- RVV implementation

tests/
- Host-side verification
- QEMU-side verification

scripts/
- Build and execution automation

docs/
- Progress tracking
- Design decisions
- Measurements
- Final report

## Scalar Pipeline Components

Image I/O
- Raw grayscale image loading

Filtering
- Generic convolution
- Gaussian blur

Gradient Computation
- Sobel X
- Sobel Y

Edge Analysis
- Magnitude L1
- Magnitude L2
- Direction Quantization

Canny Stages
- Non-Maximum Suppression
- Double Threshold
- Hysteresis

## Future RVV Strategy

The scalar implementation serves as the reference baseline.

RVV implementations will be compared against the scalar baseline for:
- Correctness
- Performance
- Binary size
- Vector-length scalability

