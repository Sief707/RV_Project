#include <gtest/gtest.h>

#include "image/image.hpp"
#include "filters/hysterisis.hpp"

TEST(Hysteresis, PromotesConnectedWeakEdge)
{
    Image input(3, 3);

    input.pixels =
    {
          0,   0,   0,
          0, 255, 128,
          0,   0,   0
    };

    Image output =
        hysteresis(input);

    EXPECT_EQ(output.pixels[1 * 3 + 1], 255);
    EXPECT_EQ(output.pixels[1 * 3 + 2], 255);
}

TEST(Hysteresis, RemovesIsolatedWeakEdge)
{
    Image input(3, 3);

    input.pixels =
    {
          0,   0,   0,
          0, 128,   0,
          0,   0,   0
    };

    Image output =
        hysteresis(input);

    EXPECT_EQ(output.pixels[1 * 3 + 1], 0);
}
