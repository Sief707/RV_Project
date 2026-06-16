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
#include <vector>

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

static void save_grid_ppm(const std::string& filename,
const std::vector<Stage>& stages,
const std::string& pipeline_title)
{
// layout: 3 columns, 2 rows (for 6 stages)
const uint32_t COLS = 3;
const uint32_t ROWS = (stages.size() + COLS - 1) / COLS;
const uint32_t CELL_W = stages[0].img.width;
const uint32_t CELL_H = stages[0].img.height;
const uint32_t LABEL_H = 20; // pixels reserved for text label
const uint32_t PADDING = 10;

const uint32_t TOTAL_W = COLS * (CELL_W + PADDING) + PADDING;
const uint32_t TOTAL_H = ROWS * (CELL_H + LABEL_H + PADDING) + PADDING + 30;
// +30 for top title

// create a flat RGB buffer (white background)
std::vector<uint8_t> canvas(TOTAL_W * TOTAL_H * 3, 30); // dark background

// helper: draw a grayscale image into the canvas at (ox, oy)
auto draw_image = [&](const Image& img, uint32_t ox, uint32_t oy)
{
for (uint32_t r = 0; r < img.height; ++r)
for (uint32_t c = 0; c < img.width; ++c)
{
uint8_t v = img.pixels[r * img.width + c];
uint32_t base = ((oy + r) * TOTAL_W + (ox + c)) * 3;
canvas[base] = v;
canvas[base + 1] = v;
canvas[base + 2] = v;
}
};

// place each stage in its cell
for (uint32_t s = 0; s < stages.size(); ++s)
{
uint32_t col = s % COLS;
uint32_t row = s / COLS;
uint32_t ox = PADDING + col * (CELL_W + PADDING);
uint32_t oy = 30 + PADDING + row * (CELL_H + LABEL_H + PADDING) + LABEL_H;
draw_image(stages[s].img, ox, oy);
}

// save as PPM (P6 = color)
std::ofstream file(filename, std::ios::binary);
file << "P6\n" << TOTAL_W << " " << TOTAL_H << "\n255\n";
file.write(reinterpret_cast<const char*>(canvas.data()), canvas.size());

printf("Saved grid: %s\n", filename.c_str());
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
printf("╔════════════════════════════════════════════════════════════╗\n");
printf("║ L1 vs L2 Magnitude — Full Pipeline Report ║\n");
printf("╠════════════════════════════════════════════════════════════╣\n");
printf("║ %-22s %-17s %-17s ║\n", "Stage", "L1", "L2");
printf("╠════════════════════════════════════════════════════════════╣\n");
printf("║ %-22s %-17.2f %-17.2f ║\n", "Magnitude avg", avg_val(mag_l1), avg_val(mag_l2));
printf("║ %-22s %-17u %-17u ║\n", "Magnitude nonzero", count_nonzero(mag_l1), count_nonzero(mag_l2));
printf("║ %-22s %-17u %-17u ║\n", "After NMS nonzero", count_nonzero(thin_l1), count_nonzero(thin_l2));
printf("║ %-22s %-17u %-17u ║\n", "After threshold", count_nonzero(thresh_l1), count_nonzero(thresh_l2));
printf("║ %-22s %-17u %-17u ║\n", "Final edges", count_nonzero(edges_l1), count_nonzero(edges_l2));
printf("╠════════════════════════════════════════════════════════════╣\n");
printf("║ %-22s %-17s %-17s ║\n", "Formula", "|Gx|+|Gy|", "sqrt(Gx^2+Gy^2)");
printf("║ %-22s %-17s %-17s ║\n", "Speed", "faster", "slower");
printf("║ %-22s %-17s %-17s ║\n", "Accuracy", "approximate", "exact");
printf("║ %-22s %-17s %-17s ║\n", "Standard Canny?", "No", "Yes");
printf("╚════════════════════════════════════════════════════════════╝\n");

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

std::vector<Stage> stages_I1 = {
{ "Input", input },
{ "Gaussian Blur", blurred },
{ "Magnitude", magnitude_l1 },
{ "NMS", thinned_l1 },
{ "Double Threshold", thresholded_I1},
{ "Final Edges", edges_l1 }
};

std::vector<Stage> stages_I2 = {
{ "Input", input },
{ "Gaussian Blur", blurred },
{ "Magnitude", magnitude_l2 },
{ "NMS", thinned_l2 },
{ "Double Threshold", thresholded_I2},
{ "Final Edges", edges_l2 }
};

save_grid_ppm("images/pipeline_grid_I1.ppm", stages_I1, "Canny Pipeline_I1");
save_grid_ppm("images/pipeline_grid_I2.ppm", stages_I2, "Canny Pipeline_I2");


(void)magnitude_l1;
(void)magnitude_l2;
(void)direction;
}