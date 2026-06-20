#ifndef RVV_SOBEL_HPP
#define RVV_SOBEL_HPP

#include "image/image.hpp"

ImageInt32 rvv_sobel_x(const Image& input);

ImageInt32 rvv_sobel_y(const Image& input);

#endif
