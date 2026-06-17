#include "filters/threshold.hpp"

// These are the three possible output values
static const uint8_t STRONG = 255;
static const uint8_t WEAK   = 128;
static const uint8_t NONE   =   0;

Image double_threshold(const Image& input,
                       uint8_t low,
                       uint8_t high)
{
    Image output(input.width, input.height);

    for (uint32_t i = 0; i < input.pixels.size(); ++i)
    {
        const uint8_t magnitude = input.pixels[i];

        if (magnitude >= high)
        {
            // Definitely an edge — strong and clear
            output.pixels[i] = STRONG;
        }
        else if (magnitude >= low)
        {
            // Maybe an edge — hysteresis will decide
            output.pixels[i] = WEAK;
        }
        else
        {
            // Too weak — definitely not an edge
            output.pixels[i] = NONE;
        }
    }

    return output;
}