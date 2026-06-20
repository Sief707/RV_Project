#include "rvv/rvv_magnitude.hpp"

#include <algorithm>
#include <cstdint>

#include <riscv_vector.h>

Image rvv_gradient_magnitude_l1(
    const ImageInt32& gx,
    const ImageInt32& gy)
{
    Image output(
        gx.width,
        gx.height);

    const uint32_t N =
        gx.pixels.size();

    uint32_t i = 0;

    while(i < N)
    {
        size_t vl =
            __riscv_vsetvl_e32m1(
                N - i);

        for(size_t j = 0; j < vl; ++j)
        {
            int32_t value =
                std::abs(gx.pixels[i + j]) +
                std::abs(gy.pixels[i + j]);

            value =
                std::clamp(
                    value,
                    0,
                    255);

            output.pixels[i + j] =
                static_cast<uint8_t>(
                    value);
        }

        i += vl;
    }

    return output;
}
