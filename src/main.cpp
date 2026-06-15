#include <iostream>

#include "image/image.hpp"
#include "pipeline/pipeline.hpp"

int main()
{
    const uint32_t W = 512;
    const uint32_t H = 512;

    Image image(W, H);
    
    if (!image.load_raw("images/input.raw"))
    {
        std::cerr << "ERROR: could not load images/input.raw\n";
        return 1;
    }

    run_pipeline(image);

    std::cout
        << "Pipeline executed successfully."
        << std::endl;

    return 0;
}
