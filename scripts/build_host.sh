#!/bin/bash

set -e

mkdir -p build-host

g++ src/main.cpp \
    -o build-host/rv_project_host

echo "Host build completed successfully."
