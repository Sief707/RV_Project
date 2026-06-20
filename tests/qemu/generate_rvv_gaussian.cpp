#include <cstdio>

#include "config.hpp"
#include "image/image.hpp"
#include "rvv/rvv_gaussian.hpp"

int main()
{
    printf("Start\n");

    Image input(
        IMAGE_WIDTH,
        IMAGE_HEIGHT);

    if(!input.load_raw("/tmp/input.raw"))
    {
        printf("LOAD FAILED\n");
        return 1;
    }

    printf("Image loaded\n");

    Image output =
        rvv_gaussian_blur(input);

    printf("Gaussian finished\n");

    if(!output.save_raw("/tmp/rvv_gaussian.raw"))
    {
        printf("SAVE FAILED\n");
        return 2;
    }

    printf("SAVE OK\n");

    return 0;
}
