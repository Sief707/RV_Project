#!/bin/bash

set -e

mkdir -p build-host

g++ tests/host/test_gaussian.cpp \
    src/image/image.cpp \
    src/filters/gaussian.cpp \
    -Iinclude \
    -lgtest -lgtest_main -lpthread \
    -o build-host/test_gaussian

g++ tests/host/test_sobel.cpp \
    src/image/image.cpp \
    src/filters/sobel.cpp \
    -Iinclude \
    -lgtest -lgtest_main -lpthread \
    -o build-host/test_sobel

g++ tests/host/test_magnitude.cpp \
    src/image/image.cpp \
    src/filters/magnitude.cpp \
    -Iinclude \
    -lgtest -lgtest_main -lpthread \
    -o build-host/test_magnitude

g++ tests/host/test_direction.cpp \
    src/image/image.cpp \
    src/filters/direction.cpp \
    -Iinclude \
    -lgtest -lgtest_main -lpthread \
    -o build-host/test_direction

g++ tests/host/test_nms.cpp \
    src/image/image.cpp \
    src/filters/NMS.cpp \
    -Iinclude \
    -lgtest -lgtest_main -lpthread \
    -o build-host/test_nms

g++ tests/host/test_threshold.cpp \
    src/image/image.cpp \
    src/filters/threshold.cpp \
    -Iinclude \
    -lgtest -lgtest_main -lpthread \
    -o build-host/test_threshold

g++ tests/host/test_hysteresis.cpp \
    src/image/image.cpp \
    src/filters/hysterisis.cpp \
    -Iinclude \
    -lgtest -lgtest_main -lpthread \
    -o build-host/test_hysteresis

echo "Host tests built successfully."
