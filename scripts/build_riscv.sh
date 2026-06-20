#!/bin/bash

set -e

mkdir -p build-riscv

# Changed to the proper bare-metal compiler triple
riscv64-unknown-elf-g++ -static \
    src/main.cpp \
    src/image/image.cpp \
    src/filters/gaussian.cpp \
    src/filters/sobel.cpp \
    src/filters/magnitude.cpp \
    src/filters/direction.cpp \
    src/filters/direction_fast.cpp \
    src/rvv/rvv_gaussian.cpp \
    src/rvv/rvv_magnitude.cpp \
    src/filters/NMS.cpp \
    src/filters/threshold.cpp \
    src/filters/hysterisis.cpp \
    src/pipeline/pipeline.cpp \
    benchmarks/syscalls.cpp \
    benchmarks/timing.cpp \
    -Iinclude \
    -Ibenchmarks \
    -march=rv64gcv \
    -mabi=lp64d \
    ${OPT_LEVEL:- -O0} \
    -o build-riscv/rv_project_riscv

echo "RISC-V build completed successfully with optimization ${OPT_LEVEL:- -O0}."