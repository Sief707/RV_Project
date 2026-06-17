#!/bin/bash

set -e

for test in build-host/test_*; do
    echo "===================="
    echo "$test"
    "$test"
done

echo "All tests passed."
