#include "filters/magnitude.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>

Image gradient_magnitude_l1(
    const ImageInt32& gx,
    const ImageInt32& gy)
{
    Image output(gx.width, gx.height);

    for (uint32_t i = 0; i < gx.pixels.size(); ++i)
    {
        const int32_t value =
            std::abs(gx.pixels[i]) +
            std::abs(gy.pixels[i]);

        output.pixels[i] =
            static_cast<uint8_t>(
                std::clamp(value, 0, 255));
    }

    return output;
}

Image gradient_magnitude_l2(
    const ImageInt32& gx,
    const ImageInt32& gy)
{
    Image output(gx.width, gx.height);

    for (uint32_t i = 0; i < gx.pixels.size(); ++i)
    {
        const double magnitude =
            std::sqrt(
                static_cast<double>(gx.pixels[i]) *
                static_cast<double>(gx.pixels[i]) +
                static_cast<double>(gy.pixels[i]) *
                static_cast<double>(gy.pixels[i]));

        const int32_t value =
            static_cast<int32_t>(magnitude);

        output.pixels[i] =
            static_cast<uint8_t>(
                std::clamp(value, 0, 255));
    }

    return output;
}
