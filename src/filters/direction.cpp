#include "filters/direction.hpp"

#include <cmath>
#include <cstdint>

Image gradient_direction(
    const ImageInt32& gx,
    const ImageInt32& gy)
{
    Image output(gx.width, gx.height);

    for (uint32_t i = 0; i < gx.pixels.size(); ++i)
    {
        double angle =
            std::atan2(
                static_cast<double>(gy.pixels[i]),
                static_cast<double>(gx.pixels[i]))
            * 180.0 / M_PI;

        if (angle < 0.0)
        {
            angle += 180.0;
        }

        uint8_t direction;

        if (angle < 22.5 || angle >= 157.5)
        {
            direction = 0;
        }
        else if (angle < 67.5)
        {
            direction = 45;
        }
        else if (angle < 112.5)
        {
            direction = 90;
        }
        else
        {
            direction = 135;
        }

        output.pixels[i] = direction;
    }

    return output;
}
