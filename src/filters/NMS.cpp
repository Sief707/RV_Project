#include "filters/NMS.hpp"

#include <cstdint>

Image non_max_suppression(const Image& magnitude,
                          const Image& direction)
{
    const uint32_t W = magnitude.width;
    const uint32_t H = magnitude.height;

    // Start with all zeros — only local peaks get written in
    Image output(W, H);

    // Skip the 1-pixel border — neighbors go out of bounds there
    for (uint32_t pixel_height = 1; pixel_height < H - 1; ++pixel_height)
    {
        for (uint32_t pixel_width = 1; pixel_width < W - 1; ++pixel_width)
        {
            const uint32_t idx = pixel_height * W + pixel_width;

            const uint8_t mag = magnitude.pixels[idx];
            const uint8_t dir = direction.pixels[idx];

            uint8_t neighbor1 = 0;
            uint8_t neighbor2 = 0;

            // Check the two neighbors perpendicular to the edge direction.
            // dir == 0   → edge is horizontal → gradient points left/right
            // dir == 45  → edge is diagonal / → gradient points top-right/bottom-left
            // dir == 90  → edge is vertical   → gradient points up/down
            // dir == 135 → edge is diagonal \ → gradient points top-left/bottom-right
            if (dir == 0)
            {
                neighbor1 = magnitude.pixels[pixel_height * W + (pixel_width - 1)];
                neighbor2 = magnitude.pixels[pixel_height * W + (pixel_width + 1)];
            }
            else if (dir == 45)
            {
                neighbor1 = magnitude.pixels[(pixel_height - 1) * W + (pixel_width + 1)];
                neighbor2 = magnitude.pixels[(pixel_height + 1) * W + (pixel_width - 1)];
            }
            else if (dir == 90)
            {
                neighbor1 = magnitude.pixels[(pixel_height - 1) * W + pixel_width];
                neighbor2 = magnitude.pixels[(pixel_height + 1) * W + pixel_width];
            }
            else
            {
                // dir == 135
                neighbor1 = magnitude.pixels[(pixel_height - 1) * W + (pixel_width - 1)];
                neighbor2 = magnitude.pixels[(pixel_height + 1) * W + (pixel_width + 1)];
            }

            // Keep this pixel only if it is the local peak
            if (mag >= neighbor1 && mag >= neighbor2)
            {
                output.pixels[idx] = mag;
            }
        }
    }

    return output;
}