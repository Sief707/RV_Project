#include <iostream>

#include "image/image.hpp"
#include "pipeline/pipeline.hpp"
#include "config.hpp"

int main()
{
    Image image(IMAGE_WIDTH, IMAGE_HEIGHT);

    // Accept width and height from command line
    
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
