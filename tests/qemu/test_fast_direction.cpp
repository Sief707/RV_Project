#include <cstdio>

#include "filters/direction.hpp"
#include "filters/direction_fast.hpp"

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

    Image scalar =
        gradient_direction(gx, gy);

    Image fast =
        fast_gradient_direction(gx, gy);

    for(size_t i=0;i<scalar.pixels.size();++i)
    {
        if(scalar.pixels[i] != fast.pixels[i])
        {
            printf(
                "FAIL at %zu: scalar=%u fast=%u\n",
                i,
                scalar.pixels[i],
                fast.pixels[i]);

            return 1;
        }
    }

    printf("PASS\n");

    return 0;
}
