#include <cstdio>

#include "image/image.hpp"
#include "rvv/rvv_gaussian.hpp"

int main()
{
    Image input(512,390);

    for(uint32_t i=0;i<input.pixels.size();++i)
    {
        input.pixels[i] =
            static_cast<uint8_t>((i * 7) & 255);
    }

    for(int iter=0; iter<50; ++iter)
    {
        Image out =
            rvv_gaussian_blur(input);

        (void)out;
    }

    printf("benchmark complete\n");

    return 0;
}
