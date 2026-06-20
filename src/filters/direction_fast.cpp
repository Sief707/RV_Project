#include "filters/direction_fast.hpp"

#include <cstdlib>

Image fast_gradient_direction(
    const ImageInt32& gx,
    const ImageInt32& gy)
{
    Image output(gx.width, gx.height);

    for(uint32_t i = 0; i < gx.pixels.size(); ++i)
    {
        int32_t gxi = gx.pixels[i];
        int32_t gyi = gy.pixels[i];

        int32_t ax = std::abs(gxi);
        int32_t ay = std::abs(gyi);

        uint8_t dir;

        if(ay * 5 < ax * 2)
        {
            dir = 0;
        }
        else if(ay * 5 > ax * 12)
        {
            dir = 90;
        }
        else
        {
            if((gxi >= 0 && gyi >= 0) ||
               (gxi < 0 && gyi < 0))
            {
                dir = 45;
            }
            else
            {
                dir = 135;
            }
        }

        output.pixels[i] = dir;
    }

    return output;
}
