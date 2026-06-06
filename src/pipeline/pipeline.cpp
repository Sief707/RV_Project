#include "pipeline/pipeline.hpp"

#include "filters/direction.hpp"
#include "filters/gaussian.hpp"
#include "filters/magnitude.hpp"
#include "filters/sobel.hpp"

void run_pipeline(const Image& input)
{
    Image blurred =
        gaussian_blur(input);

    ImageInt32 gx =
        sobel_x(blurred);

    ImageInt32 gy =
        sobel_y(blurred);

    Image magnitude_l1 =
        gradient_magnitude_l1(
            gx,
            gy);

    Image magnitude_l2 =
        gradient_magnitude_l2(
            gx,
            gy);

    Image direction =
        gradient_direction(
            gx,
            gy);

    (void)magnitude_l1;
    (void)magnitude_l2;
    (void)direction;
}
