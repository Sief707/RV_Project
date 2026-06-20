#include "rvv/rvv_sobel.hpp"

#include <riscv_vector.h>

#include "filters/sobel.hpp"

ImageInt32 rvv_sobel_x(const Image& input)
{
    ImageInt32 output(
        input.width,
        input.height);

    const uint32_t W = input.width;
    const uint32_t H = input.height;

    ImageInt32 scalar_ref =
        sobel_x(input);

    for(uint32_t y = 0; y < H; ++y)
    {
        for(uint32_t x = 0; x < W; ++x)
        {
            if(y == 0 || y == H - 1 ||
               x == 0 || x == W - 1)
            {
                output.pixels[y*W+x] =
                    scalar_ref.pixels[y*W+x];
            }
        }
    }

    for(uint32_t y = 1; y < H - 1; ++y)
    {
        uint32_t x = 1;

        while(x < W - 1)
        {
            size_t vl =
                __riscv_vsetvl_e8m1(
                    (W - 1) - x);

            const uint8_t* r0 =
                &input.pixels[(y - 1) * W];

            const uint8_t* r1 =
                &input.pixels[(y    ) * W];

            const uint8_t* r2 =
                &input.pixels[(y + 1) * W];

            vuint8m1_t tl =
                __riscv_vle8_v_u8m1(r0 + x - 1, vl);

            vuint8m1_t tr =
                __riscv_vle8_v_u8m1(r0 + x + 1, vl);

            vuint8m1_t ml =
                __riscv_vle8_v_u8m1(r1 + x - 1, vl);

            vuint8m1_t mr =
                __riscv_vle8_v_u8m1(r1 + x + 1, vl);

            vuint8m1_t bl =
                __riscv_vle8_v_u8m1(r2 + x - 1, vl);

            vuint8m1_t br =
                __riscv_vle8_v_u8m1(r2 + x + 1, vl);

            uint8_t a[256];
            uint8_t b[256];
            uint8_t c[256];
            uint8_t d[256];
            uint8_t e[256];
            uint8_t f[256];

            __riscv_vse8_v_u8m1(a, tl, vl);
            __riscv_vse8_v_u8m1(b, tr, vl);
            __riscv_vse8_v_u8m1(c, ml, vl);
            __riscv_vse8_v_u8m1(d, mr, vl);
            __riscv_vse8_v_u8m1(e, bl, vl);
            __riscv_vse8_v_u8m1(f, br, vl);

            for(size_t i=0;i<vl;++i)
            {
                output.pixels[y*W+x+i] =
                    (-a[i] + b[i]) +
                    (-2*c[i] + 2*d[i]) +
                    (-e[i] + f[i]);
            }

            x += vl;
        }
    }

    return output;
}

ImageInt32 rvv_sobel_y(const Image& input)
{
    return sobel_y(input);
}
