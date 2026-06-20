#include <cstdio>

#include "config.hpp"
#include "image/image.hpp"
#include "filters/gaussian.hpp"
#include "rvv/rvv_gaussian.hpp"

int main()
{
    Image input(
        IMAGE_WIDTH,
        IMAGE_HEIGHT);

    if(!input.load_raw("images/input.raw"))
    {
        printf("LOAD FAILED\n");
        return 1;
    }

    Image scalar =
        gaussian_blur(input);

    Image rvv =
        rvv_gaussian_blur(input);

    size_t mismatches = 0;

    for(size_t i=0;i<scalar.pixels.size();++i)
    {
        if(scalar.pixels[i] != rvv.pixels[i])
        {
            ++mismatches;
        }
    }

    scalar.save_raw(
        "images/scalar_gaussian.raw");

    rvv.save_raw(
        "images/rvv_gaussian.raw");

    printf("Pixels     : %zu\n", scalar.pixels.size());
    printf("Mismatches : %zu\n", mismatches);

    return 0;
}
