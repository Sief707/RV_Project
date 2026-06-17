#!/bin/bash

set -e

echo "PWD:"
pwd

echo "Image exists?"
ls -l images/input.raw

qemu-riscv64 ./build-riscv/rv_project_riscv