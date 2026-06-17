#ifndef THRESHOLD_HPP
#define THRESHOLD_HPP

#include "image/image.hpp"

// Classifies each pixel into:
// strong edge  = 255
// weak edge    = 128
// non-edge     = 0
Image double_threshold(const Image& input,
                       uint8_t low,
                       uint8_t high);
#endif