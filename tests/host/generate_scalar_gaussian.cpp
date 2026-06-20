#include "config.hpp"
#include "image/image.hpp"
#include "filters/gaussian.hpp"

int main()
{
    Image input(
        IMAGE_WIDTH,
        IMAGE_HEIGHT);

    if(!input.load_raw("images/input.raw"))
        return 1;

    Image output =
        gaussian_blur(input);

    output.save_raw(
        "images/scalar_gaussian.raw");

    return 0;
}
