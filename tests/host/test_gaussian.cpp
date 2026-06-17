#include <gtest/gtest.h>

#include "image/image.hpp"
#include "filters/gaussian.hpp"

TEST(Gaussian, BlackImageRemainsBlack)
{
    Image input(16, 16);

    Image output =
        gaussian_blur(input);

    for (uint8_t pixel : output.pixels)
    {
        EXPECT_EQ(pixel, 0);
    }
}

TEST(Gaussian, ImpulseResponse)
{
    Image input(9, 9);

    input.pixels[4 * 9 + 4] = 255;

    Image output =
        gaussian_blur(input);

    const uint8_t expected[25] =
    {
         0,  3,  5,  3,  0,
         3, 15, 23, 15,  3,
         5, 23, 35, 23,  5,
         3, 15, 23, 15,  3,
         0,  3,  5,  3,  0
    };

    uint32_t k = 0;

    for (uint32_t y = 2; y <= 6; ++y)
    {
        for (uint32_t x = 2; x <= 6; ++x)
        {
            EXPECT_EQ(
                output.pixels[y * 9 + x],
                expected[k++]);
        }
    }
}

TEST(Gaussian, UniformImageInteriorRemainsUniform)
{
    Image input(16, 16);

    for (uint8_t& pixel : input.pixels)
    {
        pixel = 100;
    }

    Image output =
        gaussian_blur(input);

    for (uint32_t y = 2; y < 14; ++y)
    {
        for (uint32_t x = 2; x < 14; ++x)
        {
            EXPECT_EQ(
                output.pixels[y * 16 + x],
                100);
        }
    }
}
