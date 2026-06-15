#include "pipeline/pipeline.hpp"

#include "filters/direction.hpp"
#include "filters/gaussian.hpp"
#include "filters/magnitude.hpp"
#include "filters/sobel.hpp"
#include "filters/NMS.hpp"
#include "filters/threshold.hpp"
#include "filters/hysterisis.hpp"

#include <cstdio>
#include <fstream>
#include <string>
#include <algorithm>

static void save_ppm(const std::string& filename, const Image& img)
{
    std::ofstream file(filename);
    file << "P2\n" << img.width << " " << img.height << "\n255\n";

    for (uint32_t y = 0; y < img.height; ++y)
    {
        for (uint32_t x = 0; x < img.width; ++x)
        {
            file << static_cast<int>(img.pixels[y * img.width + x]);
            if (x < img.width - 1) file << " ";
        }
        file << "\n";
    }
    printf("Saved: %s\n", filename.c_str());
}

void run_pipeline(const Image& input)
{
    save_ppm("images/00_input.ppm", input);

    Image blurred =
        gaussian_blur(input);
    save_ppm("images/01_blurred.ppm", blurred);
    // DEBUG
    uint8_t max_blur = *std::max_element(blurred.pixels.begin(), blurred.pixels.end());
    printf("Stage 1 done: Gaussian blur — max pixel = %d\n", max_blur);

    ImageInt32 gx =
        sobel_x(blurred);

    ImageInt32 gy =
        sobel_y(blurred);

    // DEBUG
    int32_t max_gx = *std::max_element(gx.pixels.begin(), gx.pixels.end());
    printf("Stage 2 done: Sobel — max gx = %d\n", max_gx);

    Image magnitude_l1 =
        gradient_magnitude_l1(
            gx,
            gy);

    Image magnitude_l2 =
        gradient_magnitude_l2(
            gx,
            gy);
    save_ppm("images/02_magnitude_l1.ppm", magnitude_l1);
    save_ppm("images/02_magnitude_l2.ppm", magnitude_l2);
    printf("Stage 3 done: Magnitude L1 and L2\n");


    Image direction =
        gradient_direction(
            gx,
            gy);
    printf("Stage 4 done: Direction\n");

    Image thinned = non_max_suppression(magnitude_l2, direction);
    save_ppm("images/03_thinned.ppm", thinned);
    printf("Stage 5 done: Non-max suppression\n");


    Image thresholded = double_threshold(thinned, 50, 150);
    save_ppm("images/04_thresholded.ppm", thresholded);
    printf("Stage 6 done: Double threshold\n");

        Image edges = hysteresis(thresholded);
    save_ppm("images/05_edges.ppm", edges);
    printf("Stage 7 done: Hysteresis\n");

    printf("\nAll done! Run: eog images/ to see all stages\n");

    (void)magnitude_l1;
    (void)magnitude_l2;
    (void)direction;
}
