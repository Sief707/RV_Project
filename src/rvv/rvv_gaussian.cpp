#include "rvv/rvv_gaussian.hpp"

#include <algorithm>
#include <cstdint>

#include <riscv_vector.h>

Image rvv_gaussian_blur(const Image& input)
{
    Image output(input.width, input.height);

    const uint32_t W = input.width;
    const uint32_t H = input.height;

    static const int K[5][5] =
    {
        { 1,  4,  6,  4, 1},
        { 4, 16, 24, 16, 4},
        { 6, 24, 36, 24, 6},
        { 4, 16, 24, 16, 4},
        { 1,  4,  6,  4, 1}
    };

    for(uint32_t y = 2; y < H - 2; ++y)
    {
        uint32_t x = 2;

        while(x < W - 2)
        {
            size_t vl =
                __riscv_vsetvl_e32m4(
                    (W - 2) - x);

            vint32m4_t acc =
                __riscv_vmv_v_x_i32m4(
                    0,
                    vl);

            for(int ky = -2; ky <= 2; ++ky)
            {
                for(int kx = -2; kx <= 2; ++kx)
                {
                    int32_t temp[256];

                    for(size_t i = 0; i < vl; ++i)
                    {
                        temp[i] =
                            input.pixels[
                                (y + ky) * W +
                                (x + i + kx)]
                            *
                            K[ky + 2][kx + 2];
                    }

                    vint32m4_t v =
                        __riscv_vle32_v_i32m4(
                            temp,
                            vl);

                    acc =
                        __riscv_vadd_vv_i32m4(
                            acc,
                            v,
                            vl);
                }
            }

            int32_t sums[256];

            __riscv_vse32_v_i32m4(
                sums,
                acc,
                vl);

            for(size_t i = 0; i < vl; ++i)
            {
                int32_t value =
                    sums[i] / 256;

                value =
                    std::clamp(
                        value,
                        0,
                        255);

                output.pixels[
                    y * W + x + i] =
                    static_cast<uint8_t>(
                        value);
            }

            x += vl;
        }
    }

    return output;
}
