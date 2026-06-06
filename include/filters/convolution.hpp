#ifndef CONVOLUTION_HPP
#define CONVOLUTION_HPP

#include <cstdint>
#include <vector>

#include "image/image.hpp"

template<typename AccumulatorType>
ImageInt32 convolve(
    const Image& input,
    const std::vector<AccumulatorType>& kernel,
    uint32_t kernel_width,
    uint32_t kernel_height)
{
    ImageInt32 output(input.width, input.height);

    const int32_t kernel_center_x = kernel_width / 2;
    const int32_t kernel_center_y = kernel_height / 2;

    for (uint32_t y = 0; y < input.height; ++y)
    {
        for (uint32_t x = 0; x < input.width; ++x)
        {
            AccumulatorType sum = 0;

            for (uint32_t ky = 0; ky < kernel_height; ++ky)
            {
                for (uint32_t kx = 0; kx < kernel_width; ++kx)
                {
                    const int32_t image_x =
                        static_cast<int32_t>(x) +
                        static_cast<int32_t>(kx) -
                        kernel_center_x;

                    const int32_t image_y =
                        static_cast<int32_t>(y) +
                        static_cast<int32_t>(ky) -
                        kernel_center_y;

                    if (image_x < 0 ||
                        image_y < 0 ||
                        image_x >= static_cast<int32_t>(input.width) ||
                        image_y >= static_cast<int32_t>(input.height))
                    {
                        continue;
                    }

                    const uint8_t pixel =
                        input.pixels[
                            image_y * input.width + image_x];

                    const AccumulatorType coefficient =
                        kernel[
                            ky * kernel_width + kx];

                    sum += pixel * coefficient;
                }
            }

            output.pixels[y * input.width + x] =
                sum;
        }
    }

    return output;
}

#endif
