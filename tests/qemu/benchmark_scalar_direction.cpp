#include <cstdio>

#include "image/image.hpp"
#include "filters/direction.hpp"

int main()
{
    ImageInt32 gx(512,390);
    ImageInt32 gy(512,390);

    for(uint32_t i=0;i<gx.pixels.size();++i)
    {
        gx.pixels[i] =
            static_cast<int32_t>((i % 255) - 127);

        gy.pixels[i] =
            static_cast<int32_t>(((i * 3) % 255) - 127);
    }

    for(int iter=0; iter<50; ++iter)
    {
        Image dir =
            gradient_direction(gx, gy);

        (void)dir;
    }

    printf("benchmark complete\n");

    return 0;
}
