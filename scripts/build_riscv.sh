#!/bin/bash

set -e

mkdir -p build-riscv

riscv64-linux-gnu-g++ \
    src/main.cpp \
    src/image/image.cpp \
    src/filters/gaussian.cpp \
    src/filters/sobel.cpp \
    src/filters/magnitude.cpp \
    src/filters/direction.cpp \
    src/filters/NMS.cpp \
    src/filters/threshold.cpp \
    src/filters/hysterisis.cpp \
    src/pipeline/pipeline.cpp \
    benchmarks/timing.cpp \
    -Iinclude \
    -Ibenchmarks \
    ${OPT_LEVEL:- -O0} \
    -o build-riscv/rv_project_riscv

echo "RISC-V build completed successfully with optimization ${OPT_LEVEL:- -O0}."