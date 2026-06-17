#!/bin/bash

set -e

declare -A TESTS=(
[V11_shapes]="verification_shapes.png"
[V12_border]="border_test.png"
[V13_thin_lines]="thin_lines_test.png"
[V14_noise]="noisy_shapes.png"
[V15_low_contrast]="low_contrast.png"
[V_friend_test]="friend_reference.png"
)

for TEST in "${!TESTS[@]}"
do
    IMG="${TESTS[$TEST]}"

    echo "======================================"
    echo "Running $TEST"
    echo "======================================"

    convert "images/$IMG" -depth 8 gray:images/input.raw

    make clean >/dev/null 2>&1
    make >/dev/null 2>&1

    ./build-host/rv_project_host >/dev/null 2>&1

    for f in images/*.ppm
    do
        convert "$f" "${f%.ppm}.png"
    done

    mkdir -p "verification/$TEST"

    cp images/00_input.png               "verification/$TEST/"
    cp images/01_blurred.png             "verification/$TEST/"
    cp images/02_magnitude_l1.png        "verification/$TEST/"
    cp images/02_magnitude_l2.png        "verification/$TEST/"
    cp images/03_thinned_l1.png          "verification/$TEST/"
    cp images/03_thinned_l2.png          "verification/$TEST/"
    cp images/04_thresholded_l1.png      "verification/$TEST/"
    cp images/04_thresholded_l2.png      "verification/$TEST/"
    cp images/05_edges_l1.png            "verification/$TEST/"
    cp images/05_edges_l2.png            "verification/$TEST/"
    cp images/pipeline_grid_I1.png       "verification/$TEST/"
    cp images/pipeline_grid_I2.png       "verification/$TEST/"
    cp "images/$IMG"                     "verification/$TEST/"
done

echo
echo "ALL TESTS ARCHIVED SUCCESSFULLY"
