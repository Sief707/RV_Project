#include <iostream>

#include "image/image.hpp"
#include "pipeline/pipeline.hpp"

int main()
{
    Image image(16, 16);

    run_pipeline(image);

    std::cout
        << "Pipeline executed successfully."
        << std::endl;

    return 0;
}
