#include <cassert>

#include "filters/magnitude.hpp"
#include "rvv/rvv_magnitude.hpp"

int main()
{
    ImageInt32 gx(16,16);
    ImageInt32 gy(16,16);

    for(uint32_t i=0;i<gx.pixels.size();++i)
    {
        gx.pixels[i] =
            static_cast<int32_t>((i % 21) - 10);

        gy.pixels[i] =
            static_cast<int32_t>((i % 17) - 8);
    }

    Image scalar =
        gradient_magnitude_l1(
            gx,
            gy);

    Image rvv =
        rvv_gradient_magnitude_l1(
            gx,
            gy);

    assert(rvv.width  == scalar.width);
    assert(rvv.height == scalar.height);

    for(uint32_t i=0;i<scalar.pixels.size();++i)
    {
        assert(
            scalar.pixels[i] ==
            rvv.pixels[i]);
    }

    return 0;
}
