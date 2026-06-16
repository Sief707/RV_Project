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
#include <numeric>

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

static void print_stats(const std::string& label, const Image& img)
{
uint8_t minval = *std::min_element(img.pixels.begin(), img.pixels.end());
uint8_t maxval = *std::max_element(img.pixels.begin(), img.pixels.end());

uint64_t sum = std::accumulate(img.pixels.begin(), img.pixels.end(), (uint64_t)0);
double avg = (double)sum / img.pixels.size();

uint32_t nonzero = 0;
for (uint8_t p : img.pixels)
if (p > 0) nonzero++;

printf(" %-25s min=%3d max=%3d avg=%6.2f nonzero=%u\n",
label.c_str(), minval, maxval, avg, nonzero);
}

static void print_comparison_report(
const Image& mag_l1, const Image& mag_l2,
const Image& thin_l1, const Image& thin_l2,
const Image& thresh_l1, const Image& thresh_l2,
const Image& edges_l1, const Image& edges_l2)
{
auto count_nonzero = [](const Image& img) {
uint32_t n = 0;
for (uint8_t p : img.pixels) if (p > 0) n++;
return n;
};

auto avg_val = [](const Image& img) {
uint64_t sum = 0;
for (uint8_t p : img.pixels) sum += p;
return (double)sum / img.pixels.size();
};

printf("\n");
printf("╔══════════════════════════════════════════════════════════════╗\n");
printf("║ L1 vs L2 Magnitude — Full Pipeline Report ║\n");
printf("╠══════════════════════════════════════════════════════════════╣\n");
printf("║ %-22s %-17s %-17s ║\n", "Stage", "L1", "L2");
printf("╠══════════════════════════════════════════════════════════════╣\n");
printf("║ %-22s %-17.2f %-17.2f ║\n", "Magnitude avg", avg_val(mag_l1), avg_val(mag_l2));
printf("║ %-22s %-17u %-17u ║\n", "Magnitude nonzero", count_nonzero(mag_l1), count_nonzero(mag_l2));
printf("║ %-22s %-17u %-17u ║\n", "After NMS nonzero", count_nonzero(thin_l1), count_nonzero(thin_l2));
printf("║ %-22s %-17u %-17u ║\n", "After threshold", count_nonzero(thresh_l1), count_nonzero(thresh_l2));
printf("║ %-22s %-17u %-17u ║\n", "Final edges", count_nonzero(edges_l1), count_nonzero(edges_l2));
printf("╠══════════════════════════════════════════════════════════════╣\n");
printf("║ %-22s %-17s %-17s ║\n", "Formula", "|Gx|+|Gy|", "sqrt(Gx^2+Gy^2)");
printf("║ %-22s %-17s %-17s ║\n", "Speed", "faster", "slower");
printf("║ %-22s %-17s %-17s ║\n", "Accuracy", "approximate", "exact");
printf("║ %-22s %-17s %-17s ║\n", "Standard Canny?", "No", "Yes");
printf("╚══════════════════════════════════════════════════════════════╝\n");

printf("\n");
printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
printf(" Why two methods?\n");
printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
printf("\n");
printf(" L1 norm: M = |Gx| + |Gy|\n");
printf(" - No sqrt() call → faster on embedded hardware\n");
printf(" - Always >= L2 mathematically (triangle inequality)\n");
printf(" - Overestimates diagonal gradients\n");
printf(" - Good for real-time systems where speed matters\n");
printf("\n");
printf(" L2 norm: M = sqrt(Gx^2 + Gy^2)\n");
printf(" - True Euclidean distance in gradient space\n");
printf(" - Standard Canny algorithm uses this\n");
printf(" - More accurate edge strength measurement\n");
printf(" - sqrt() is expensive on RISC-V without FPU\n");
printf("\n");
printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
printf(" Conclusion\n");
printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
printf("\n");
printf(" Use L2 for: accuracy, standard compliance, final results\n");
printf(" Use L1 for: speed-critical embedded systems, approximation\n");
printf(" On RISC-V with RVV: L2 cost can be reduced using vector sqrt\n");
printf("\n");
}

void run_pipeline(const Image& input)
{
    save_ppm("images/00_input.ppm", input);

    // ==================== step 1: gaussian blur ==================================
    Image blurred =
        gaussian_blur(input);
    save_ppm("images/01_blurred.ppm", blurred);
    // For debugging
    uint8_t max_blur = *std::max_element(blurred.pixels.begin(), blurred.pixels.end());
    printf("Stage 1 done: Gaussian blur — max pixel = %d\n", max_blur);

    // ================== step 2: sobel gradient (Gx, Gy) ==========================
    ImageInt32 gx =
        sobel_x(blurred);

    ImageInt32 gy =
        sobel_y(blurred);
    // For debugging
    int32_t max_gx = *std::max_element(gx.pixels.begin(), gx.pixels.end());
    printf("Stage 2 done: Sobel — max gx = %d\n", max_gx);
    // get the magnitude as |Gx|+|Gy|
    Image magnitude_l1 =
        gradient_magnitude_l1(
            gx,
            gy);
    // get the magnitude as sqrt(Gx^2+Gy^2)
    Image magnitude_l2 =
        gradient_magnitude_l2(
            gx,
            gy);
    save_ppm("images/02_magnitude_l1.ppm", magnitude_l1);
    save_ppm("images/02_magnitude_l2.ppm", magnitude_l2);
    // For comparing between these 2 ways of magnitude calculation
    print_stats("L1 |Gx|+|Gy|", magnitude_l1);
    print_stats("L2 sqrt(Gx^2+Gy^2)", magnitude_l2);
    printf("Stage 3 done: Magnitude L1 and L2\n");

    // get the direction
    Image direction =
        gradient_direction(
            gx,
            gy);
    printf("Stage 4 done: Direction\n");

    // Apply all following steps on both gradient_magnitude_l1, gradient_magnitude_l2
    // to see the resulted output image obtained from using both of them

    // ====================== step 3: non max suppression (NMS) =====================
    Image thinned_l1 = non_max_suppression(magnitude_l1, direction);
    Image thinned_l2 = non_max_suppression(magnitude_l2, direction);
    save_ppm("images/03_thinned_l1.ppm", thinned_l1);
    save_ppm("images/03_thinned_l2.ppm", thinned_l2);
    printf("\n[Stage 5] Non-Max Suppression:\n");
    print_stats("NMS on L1", thinned_l1);
    print_stats("NMS on L2", thinned_l2);

    // ==================== step 4: double thresholding ==============================
    Image thresholded_I1 = double_threshold(thinned_l1, 50, 150);
    Image thresholded_I2 = double_threshold(thinned_l2, 50, 150);
    save_ppm("images/04_thresholded_l1.ppm", thresholded_I1);
    save_ppm("images/04_thresholded_l2.ppm", thresholded_I2);
    printf("\n[Stage 6] Double Threshold (low=50, high=150):\n");
    print_stats("Threshold on L1", thresholded_I1);
    print_stats("Threshold on L2", thresholded_I2);

    // ===================== step 5: hysterisis ==========================
    Image edges_l1 = hysteresis(thresholded_I1);
    Image edges_l2 = hysteresis(thresholded_I2);
    save_ppm("images/05_edges_l1.ppm", edges_l1);
    save_ppm("images/05_edges_l2.ppm", edges_l2);
    printf("\n[Stage 7] Hysteresis (final edge map):\n");
    print_stats("Final edges L1", edges_l1);
    print_stats("Final edges L2", edges_l2);

    printf("\nAll done! Run: eog images/ to see all stages\n");

    print_comparison_report( magnitude_l1, magnitude_l2,
    thinned_l1, thinned_l2, thresholded_I1, thresholded_I2,
    edges_l1, edges_l2);

    (void)magnitude_l1;
    (void)magnitude_l2;
    (void)direction;
}
