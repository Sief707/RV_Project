#include <gtest/gtest.h>

#include "image/image.hpp"
#include "filters/direction.hpp"

TEST(Direction, QuantizesCanonicalAngles)
{
    ImageInt32 gx(4,1);
    ImageInt32 gy(4,1);

    gx.pixels = { 10, 10,  0, -10 };
    gy.pixels = {  0, 10, 10,  10 };

    Image dir =
        gradient_direction(
            gx,
            gy);

    EXPECT_EQ(dir.pixels[0], 0);
    EXPECT_EQ(dir.pixels[1], 45);
    EXPECT_EQ(dir.pixels[2], 90);
    EXPECT_EQ(dir.pixels[3], 135);
}

TEST(Direction, QuantizesNegativeAngles)
{
    ImageInt32 gx(3,1);
    ImageInt32 gy(3,1);

    gx.pixels = { 10, 10, -10 };
    gy.pixels = {-10,-10, -10 };

    Image dir =
        gradient_direction(
            gx,
            gy);

    EXPECT_EQ(dir.pixels[0], 135);
    EXPECT_EQ(dir.pixels[1], 135);
    EXPECT_EQ(dir.pixels[2], 45);
}
