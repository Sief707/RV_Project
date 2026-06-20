#include <cstdio>

#include "filters/direction_fast.hpp"
#include "rvv/rvv_direction.hpp"

int main()
{
    ImageInt32 gx(8,1);
    ImageInt32 gy(8,1);

    gx.pixels = {
        10, 10, 0, -10,
        -10, 5, -5, 1
    };

    gy.pixels = {
        0, 10, 10, 10,
        -10, -5, 5, 20
    };

    Image fast =
        fast_gradient_direction(gx, gy);

    Image rvv =
        rvv_gradient_direction(gx, gy);

    for(size_t i=0;i<fast.pixels.size();++i)
    {
        if(fast.pixels[i] != rvv.pixels[i])
        {
            printf(
                "FAIL at %zu: fast=%u rvv=%u\n",
                i,
                fast.pixels[i],
                rvv.pixels[i]);

            return 1;
        }
    }

    printf("PASS\n");

    return 0;
}
