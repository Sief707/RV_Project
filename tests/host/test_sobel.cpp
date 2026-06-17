#include <gtest/gtest.h>

#include "image/image.hpp"
#include "filters/sobel.hpp"

TEST(Sobel, UniformImageHasZeroInteriorGradient)
{
    Image input(16, 16);

    for (uint8_t& pixel : input.pixels)
    {
        pixel = 100;
    }

    ImageInt32 gx =
        sobel_x(input);

    ImageInt32 gy =
        sobel_y(input);

    for (uint32_t y = 1; y < 15; ++y)
    {
        for (uint32_t x = 1; x < 15; ++x)
        {
            EXPECT_EQ(
                gx.pixels[y * 16 + x],
                0);

            EXPECT_EQ(
                gy.pixels[y * 16 + x],
                0);
        }
    }
}

TEST(Sobel, VerticalEdgeProducesPositiveGx)
{
    Image input(5, 5);

    for (uint32_t y = 0; y < 5; ++y)
    {
        for (uint32_t x = 2; x < 5; ++x)
        {
            input.pixels[y * 5 + x] = 255;
        }
    }

    ImageInt32 gx =
        sobel_x(input);

    EXPECT_EQ(gx.pixels[1 * 5 + 1], 1020);
    EXPECT_EQ(gx.pixels[2 * 5 + 1], 1020);
    EXPECT_EQ(gx.pixels[3 * 5 + 1], 1020);
}

TEST(Sobel, HorizontalEdgeProducesPositiveGy)
{
    Image input(5, 5);

    for (uint32_t y = 2; y < 5; ++y)
    {
        for (uint32_t x = 0; x < 5; ++x)
        {
            input.pixels[y * 5 + x] = 255;
        }
    }

    ImageInt32 gy =
        sobel_y(input);

    EXPECT_EQ(gy.pixels[1 * 5 + 1], 1020);
    EXPECT_EQ(gy.pixels[1 * 5 + 2], 1020);
    EXPECT_EQ(gy.pixels[1 * 5 + 3], 1020);
}
