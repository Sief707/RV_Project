#include "filters/sobel.hpp"

#include <cstdint>
#include <vector>

#include "filters/convolution.hpp"

ImageInt32 sobel_x(const Image& input)
{
    const std::vector<int32_t> kernel =
    {
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1
    };

    return convolve<int32_t>(
        input,
        kernel,
        3,
        3);
}

ImageInt32 sobel_y(const Image& input)
{
    const std::vector<int32_t> kernel =
    {
        -1, -2, -1,
         0,  0,  0,
         1,  2,  1
    };

    return convolve<int32_t>(
        input,
        kernel,
        3,
        3);
}
