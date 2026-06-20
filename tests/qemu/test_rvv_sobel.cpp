#include <cstdio>

#include "filters/sobel.hpp"
#include "rvv/rvv_sobel.hpp"

int main()
{
    Image input(16,16);

    for(uint32_t i=0;i<input.pixels.size();++i)
    {
        input.pixels[i] =
            static_cast<uint8_t>((i * 7) & 255);
    }

    ImageInt32 gx_scalar =
        sobel_x(input);

    ImageInt32 gx_rvv =
        rvv_sobel_x(input);

    for(uint32_t i=0;i<gx_scalar.pixels.size();++i)
    {
        if(gx_scalar.pixels[i] !=
           gx_rvv.pixels[i])
        {
            printf(
                "idx=%u scalar=%d rvv=%d\n",
                i,
                gx_scalar.pixels[i],
                gx_rvv.pixels[i]);

            return 1;
        }
    }

    printf("PASS\n");

    return 0;
}
