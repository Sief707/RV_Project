#include <gtest/gtest.h>

#include "image/image.hpp"
#include "filters/NMS.hpp"

TEST(NMS, PreservesLocalMaximum)
{
    Image magnitude(3, 3);
    Image direction(3, 3);

    magnitude.pixels =
    {
         0,  0, 0,
        10, 20, 5,
         0,  0, 0
    };

    direction.pixels =
    {
        0, 0, 0,
        0, 0, 0,
        0, 0, 0
    };

    Image output =
        non_max_suppression(
            magnitude,
            direction);

    EXPECT_EQ(output.pixels[1 * 3 + 1], 20);
}

TEST(NMS, SuppressesNonMaximum)
{
    Image magnitude(3, 3);
    Image direction(3, 3);

    magnitude.pixels =
    {
         0,  0, 0,
        20, 10, 5,
         0,  0, 0
    };

    direction.pixels =
    {
        0, 0, 0,
        0, 0, 0,
        0, 0, 0
    };

    Image output =
        non_max_suppression(
            magnitude,
            direction);

    EXPECT_EQ(output.pixels[1 * 3 + 1], 0);
}
