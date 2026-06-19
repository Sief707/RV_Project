// main phase 5
#include <iostream>
#include "image/image.hpp"
#include "pipeline/pipeline.hpp"
#include "config.hpp"
#include "timing.hpp"

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

StageTimings t = profile_pipeline(image);

std::cout << "\n========================================\n";
std::cout << " Phase 5: Per-Stage Profiling - magnitude L1\n";
std::cout << "========================================\n";

auto pct_1 = [&](double ms) { return ms / t.total_1_ms * 100.0; };

printf(" %-20s %8.3f ms (%5.1f%%)\n", "Gaussian blur", t.gaussian_ms, pct_1(t.gaussian_ms));
printf(" %-20s %8.3f ms (%5.1f%%)\n", "Sobel Gx/Gy", t.sobel_ms, pct_1(t.sobel_ms));
printf(" %-20s %8.3f ms (%5.1f%%)\n", "Magnitude L2", t.magnitude1_ms, pct_1(t.magnitude1_ms));
printf(" %-20s %8.3f ms (%5.1f%%)\n", "Direction", t.direction_ms, pct_1(t.direction_ms));
printf(" %-20s %8.3f ms (%5.1f%%)\n", "NMS", t.nms1_ms, pct_1(t.nms1_ms));
printf(" %-20s %8.3f ms (%5.1f%%)\n", "Double threshold", t.threshold1_ms, pct_1(t.threshold1_ms));
printf(" %-20s %8.3f ms (%5.1f%%)\n", "Hysteresis", t.hysteresis1_ms,pct_1(t.hysteresis1_ms));
printf(" %-20s %8.3f ms (100.0%%)\n", "TOTAL", t.total_1_ms);

std::cout << "========================================\n";
std::cout << "\nHotspots (>20%% of total time):\n";
if (pct_1(t.magnitude1_ms) > 20) printf(" -> Magnitude %.1f%% — OPTIMIZE WITH RVV\n", pct_1(t.magnitude1_ms));
    
std::cout << "\n========================================\n";
std::cout << " Phase 5: Per-Stage Profiling - magnitude L2\n";
std::cout << "========================================\n";

auto pct_2 = [&](double ms) { return ms / t.total_2_ms * 100.0; };

printf(" %-20s %8.3f ms (%5.1f%%)\n", "Gaussian blur", t.gaussian_ms, pct_2(t.gaussian_ms));
printf(" %-20s %8.3f ms (%5.1f%%)\n", "Sobel Gx/Gy", t.sobel_ms, pct_2(t.sobel_ms));
printf(" %-20s %8.3f ms (%5.1f%%)\n", "Magnitude L2", t.magnitude2_ms, pct_2(t.magnitude2_ms));
printf(" %-20s %8.3f ms (%5.1f%%)\n", "Direction", t.direction_ms, pct_2(t.direction_ms));
printf(" %-20s %8.3f ms (%5.1f%%)\n", "NMS", t.nms2_ms, pct_2(t.nms2_ms));
printf(" %-20s %8.3f ms (%5.1f%%)\n", "Double threshold", t.threshold2_ms, pct_2(t.threshold2_ms));
printf(" %-20s %8.3f ms (%5.1f%%)\n", "Hysteresis", t.hysteresis2_ms,pct_2(t.hysteresis2_ms));
printf(" %-20s %8.3f ms (100.0%%)\n", "TOTAL", t.total_2_ms);

std::cout << "========================================\n";
std::cout << "\nHotspots (>20%% of total time):\n";
if (pct_2(t.magnitude2_ms) > 20) printf(" -> Magnitude %.1f%% — OPTIMIZE WITH RVV\n", pct_2(t.magnitude2_ms));

if (pct_2(t.gaussian_ms) > 20) printf(" -> Gaussian blur %.1f%% — OPTIMIZE WITH RVV\n", pct_2(t.gaussian_ms));
if (pct_2(t.sobel_ms) > 20) printf(" -> Sobel %.1f%% — OPTIMIZE WITH RVV\n", pct_2(t.sobel_ms));
if (pct_2(t.direction_ms) > 20) printf(" -> Sobel %.1f%% — OPTIMIZE WITH RVV\n", pct_2(t.direction_ms));

return 0;
}