# D002 - Build System

## Objective

Provide a reproducible build process for both host execution and RISC-V execution.

## Build Directories

build-host/
build-riscv/

Generated binaries must not be placed in the repository root.

---

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

---

## RISC-V Build

Script:

scripts/build_riscv.sh

Output:

build-riscv/rv_project_riscv

Verification:

PASS

The script successfully compiles:

src/main.cpp

and generates:

build-riscv/rv_project_riscv

The generated file was verified as a valid RISC-V ELF executable.

---

## QEMU Execution

Script:

scripts/run_qemu.sh

Verification:

PASS

The script successfully executes:

build-riscv/rv_project_riscv

using:

qemu-riscv64

and produces the expected output.

---

## Decision

All project builds shall be performed through scripts located in:

scripts/

Build artifacts shall be stored only inside:

build-host/
build-riscv/

The complete automated flow is:

src/main.cpp
    ↓
build_host.sh
    ↓
build-host/rv_project_host

src/main.cpp
    ↓
build_riscv.sh
    ↓
build-riscv/rv_project_riscv
    ↓
run_qemu.sh
    ↓
QEMU Execution

This keeps the repository clean and ensures reproducible builds.

---

## Makefile

The project provides a top-level Makefile that serves as the primary interface for the build system.

Supported targets:

make host
make riscv
make run
make clean

Verification:

PASS

All targets were successfully executed and verified.

The Makefile delegates build and execution operations to the corresponding scripts located in:

scripts/

This avoids duplication of build logic and provides a single user-friendly entry point for the project.

