#include <iostream>

#include "image/image.hpp"
#include "filters/magnitude.hpp"

int main()
{
    ImageInt32 gx(3, 1);
    ImageInt32 gy(3, 1);

    gx.pixels = {3, 5, 8};
    gy.pixels = {4,12,15};

    Image l1 =
        gradient_magnitude_l1(
            gx,
            gy);

    Image l2 =
        gradient_magnitude_l2(
            gx,
            gy);

    std::cout << "L1: ";

    for (uint8_t value : l1.pixels)
    {
        std::cout
            << static_cast<int>(value)
            << " ";
    }

    std::cout << std::endl;

    std::cout << "L2: ";

    for (uint8_t value : l2.pixels)
    {
        std::cout
            << static_cast<int>(value)
            << " ";
    }

    std::cout << std::endl;

    return 0;
}
