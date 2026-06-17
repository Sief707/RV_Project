#include <gtest/gtest.h>

#include "image/image.hpp"
#include "filters/threshold.hpp"

TEST(Threshold, ClassifiesPixelsCorrectly)
{
    Image input(3, 1);

    input.pixels =
    {
        30,
        100,
        200
    };

    Image output =
        double_threshold(
            input,
            50,
            150);

    EXPECT_EQ(output.pixels[0], 0);
    EXPECT_EQ(output.pixels[1], 128);
    EXPECT_EQ(output.pixels[2], 255);
}
