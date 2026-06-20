#include <cstdio>

#include "image/image.hpp"
#include "rvv/rvv_magnitude.hpp"

int main()
{
    ImageInt32 gx(512,390);
    ImageInt32 gy(512,390);

    for(uint32_t i=0;i<gx.pixels.size();++i)
    {
        gx.pixels[i] =
            static_cast<int32_t>((i % 511) - 255);

        gy.pixels[i] =
            static_cast<int32_t>((i % 401) - 200);
    }

    for(int iter=0; iter<100; ++iter)
    {
        Image out =
            rvv_gradient_magnitude_l1(
                gx,
                gy);

        (void)out;
    }

    printf("benchmark complete\n");

    return 0;
}
