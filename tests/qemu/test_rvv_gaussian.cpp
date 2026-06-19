#include <cstdio>

#include "filters/gaussian.hpp"
#include "rvv/rvv_gaussian.hpp"

int main()
{
    Image input(16,16);

    for(uint32_t i=0;i<input.pixels.size();++i)
    {
        input.pixels[i] =
            static_cast<uint8_t>((i * 7) & 255);
    }

    Image scalar =
        gaussian_blur(input);

    Image rvv =
        rvv_gaussian_blur(input);

    for(uint32_t y=2;y<14;++y)
    {
        for(uint32_t x=2;x<14;++x)
        {
            uint32_t idx =
                y*16+x;

            if(scalar.pixels[idx] !=
               rvv.pixels[idx])
            {
                printf(
                    "Mismatch (%u,%u): %u %u\n",
                    x,
                    y,
                    scalar.pixels[idx],
                    rvv.pixels[idx]);

                return 1;
            }
        }
    }

    printf("PASS\n");
    return 0;
}
