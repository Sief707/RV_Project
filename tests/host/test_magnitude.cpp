#include <gtest/gtest.h>

#include "image/image.hpp"
#include "filters/magnitude.hpp"

TEST(Magnitude, L1AndL2ReferenceValues)
{
    ImageInt32 gx(3, 1);
    ImageInt32 gy(3, 1);

    gx.pixels = {3, 5, 8};
    gy.pixels = {4, 12, 15};

    Image l1 =
        gradient_magnitude_l1(
            gx,
            gy);

    Image l2 =
        gradient_magnitude_l2(
            gx,
            gy);

    EXPECT_EQ(l1.pixels[0], 7);
    EXPECT_EQ(l1.pixels[1], 17);
    EXPECT_EQ(l1.pixels[2], 23);

    EXPECT_EQ(l2.pixels[0], 5);
    EXPECT_EQ(l2.pixels[1], 13);
    EXPECT_EQ(l2.pixels[2], 17);
}

TEST(Magnitude, SaturatesAt255)
{
    ImageInt32 gx(1, 1);
    ImageInt32 gy(1, 1);

    gx.pixels[0] = 1000;
    gy.pixels[0] = 1000;

    Image l1 =
        gradient_magnitude_l1(
            gx,
            gy);

    Image l2 =
        gradient_magnitude_l2(
            gx,
            gy);

    EXPECT_EQ(l1.pixels[0], 255);
    EXPECT_EQ(l2.pixels[0], 255);
}
