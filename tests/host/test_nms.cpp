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

TEST(NMS, PreservesVerticalMaximum)
{
    Image magnitude(3, 3);
    Image direction(3, 3);

    magnitude.pixels =
    {
         0, 10, 0,
         0, 20, 0,
         0,  5, 0
    };

    direction.pixels =
    {
         0,  0, 0,
         0, 90, 0,
         0,  0, 0
    };

    Image output =
        non_max_suppression(
            magnitude,
            direction);

    EXPECT_EQ(output.pixels[1 * 3 + 1], 20);
}

TEST(NMS, PreservesDiagonal45Maximum)
{
    Image magnitude(3, 3);
    Image direction(3, 3);

    magnitude.pixels =
    {
         0,  0, 10,
         0, 20,  0,
         5,  0,  0
    };

    direction.pixels =
    {
         0,  0, 0,
         0, 45, 0,
         0,  0, 0
    };

    Image output =
        non_max_suppression(
            magnitude,
            direction);

    EXPECT_EQ(output.pixels[1 * 3 + 1], 20);
}


TEST(NMS, PreservesDiagonal135Maximum)
{
    Image magnitude(3, 3);
    Image direction(3, 3);

    magnitude.pixels =
    {
        10,  0,  0,
         0, 20,  0,
         0,  0,  5
    };

    direction.pixels =
    {
         0,   0, 0,
         0, 135, 0,
         0,   0, 0
    };

    Image output =
        non_max_suppression(
            magnitude,
            direction);

    EXPECT_EQ(output.pixels[1 * 3 + 1], 20);
}
