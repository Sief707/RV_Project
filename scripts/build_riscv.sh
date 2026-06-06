#!/bin/bash

set -e

mkdir -p build-riscv

riscv64-unknown-elf-g++ src/main.cpp \
    -o build-riscv/rv_project_riscv

echo "RISC-V build completed successfully."
