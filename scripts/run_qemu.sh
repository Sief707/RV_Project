#!/bin/bash
set -e

qemu-riscv64 -cpu rv64,v=true,vlen=128 ./build-riscv/rv_project_riscv
