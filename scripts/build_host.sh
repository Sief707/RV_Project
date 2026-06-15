#!/bin/bash

set -e

mkdir -p build-host

g++ \
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
    -Iinclude \
    -o build-host/rv_project_host

echo "Host build completed successfully."
