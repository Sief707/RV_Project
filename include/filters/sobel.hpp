#ifndef SOBEL_HPP
#define SOBEL_HPP

#include "image/image.hpp"

ImageInt32 sobel_x(const Image& input);

ImageInt32 sobel_y(const Image& input);

#endif
