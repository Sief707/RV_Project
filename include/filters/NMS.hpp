#ifndef NMS_HPP
#define NMS_HPP

#include "image/image.hpp"

// Takes the magnitude image and the quantized direction image.
// Returns a new Image with edges thinned to 1 pixel wide.
Image non_max_suppression(const Image& magnitude,
                          const Image& direction);

#endif