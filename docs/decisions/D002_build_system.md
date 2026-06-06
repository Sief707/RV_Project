# D002 - Build System

## Objective

Provide a reproducible build process for both host execution and RISC-V execution.

## Build Directories

build-host/
build-riscv/

Generated binaries must not be placed in the repository root.

## Host Build

Script:

scripts/build_host.sh

Output:

build-host/rv_project_host

Verification:

PASS

The script successfully compiles:

src/main.cpp

and generates:

build-host/rv_project_host

The executable was executed successfully.

## Future RISC-V Build

Script:

scripts/build_riscv.sh

Output:

build-riscv/rv_project_riscv

Status:

Planned

## Future QEMU Execution

Script:

scripts/run_qemu.sh

Status:

Planned

## Decision

All project builds shall be performed through scripts located in:

scripts/

Build artifacts shall be stored only inside:

build-host/
build-riscv/

This keeps the repository clean and ensures reproducible builds.

