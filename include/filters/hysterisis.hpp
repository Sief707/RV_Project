#ifndef HYSTERESIS_HPP
#define HYSTERESIS_HPP

#include "image/image.hpp"

// Promotes weak edges connected to strong edges → final edge map
// Discards weak edges that are isolated → they are noise
Image hysteresis(const Image& input);

#endif