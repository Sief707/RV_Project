# RV_Project

RISC-V Vector Extension (RVV) implementation of the Canny Edge Detection pipeline for the Embedded Systems course.

## Project Goals

- Establish a verified scalar baseline implementation.
- Evaluate compiler optimization effects.
- Profile pipeline stages.
- Implement selected kernels using RVV intrinsics.
- Compare scalar, compiler-optimized, and RVV implementations.

## Current Status

Completed:

- Environment setup
- Host build infrastructure
- RISC-V build infrastructure
- QEMU execution infrastructure
- Image I/O
- Gaussian blur
- Sobel gradients
- Gradient magnitude (L1 and L2)
- Direction quantization
- Non-Maximum Suppression
- Double Thresholding
- Hysteresis Edge Tracking
- Full scalar pipeline integration

In Progress:

- Testing infrastructure
- Verification suite

Pending:

- Optimization sweep
- Profiling
- RVV implementation
- Final report

## Repository Structure

- include/      Header files
- src/          Source files
- tests/        Host and QEMU tests
- scripts/      Build and execution scripts
- docs/         Documentation and reports
- benchmarks/   Performance measurement utilities

## Team Workflow

feature/* → Pull Request → develop → Pull Request → main

Direct commits to develop and main are prohibited.

