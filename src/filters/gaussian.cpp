#include "filters/gaussian.hpp"

#include <algorithm>
#include <cstdint>
#include <vector>

#include "filters/convolution.hpp"

Image gaussian_blur(const Image& input)
{
    const std::vector<int32_t> kernel =
    {
         1,  4,  6,  4, 1,
         4, 16, 24, 16, 4,
         6, 24, 36, 24, 6,
         4, 16, 24, 16, 4,
         1,  4,  6,  4, 1
    };

    ImageInt32 filtered =
        convolve<int32_t>(
            input,
            kernel,
            5,
            5);

    Image output(input.width, input.height);

    for (uint32_t i = 0; i < filtered.pixels.size(); ++i)
    {
        int32_t value = filtered.pixels[i] / 256;

        value = std::clamp(value, 0, 255);

        output.pixels[i] =
            static_cast<uint8_t>(value);
    }

    return output;
}
