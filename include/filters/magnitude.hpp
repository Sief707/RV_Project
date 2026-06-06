#ifndef MAGNITUDE_HPP
#define MAGNITUDE_HPP

#include "image/image.hpp"

Image gradient_magnitude_l1(
    const ImageInt32& gx,
    const ImageInt32& gy);

Image gradient_magnitude_l2(
    const ImageInt32& gx,
    const ImageInt32& gy);

#endif
