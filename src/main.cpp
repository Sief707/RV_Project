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
std::cout << " Phase 5: Per-Stage Profiling\n";
std::cout << "========================================\n";

auto pct = [&](double ms) { return ms / t.total_ms * 100.0; };

printf(" %-20s %8.3f ms (%5.1f%%)\n", "Gaussian blur", t.gaussian_ms, pct(t.gaussian_ms));
printf(" %-20s %8.3f ms (%5.1f%%)\n", "Sobel Gx/Gy", t.sobel_ms, pct(t.sobel_ms));
printf(" %-20s %8.3f ms (%5.1f%%)\n", "Magnitude L2", t.magnitude_ms, pct(t.magnitude_ms));
printf(" %-20s %8.3f ms (%5.1f%%)\n", "Direction", t.direction_ms, pct(t.direction_ms));
printf(" %-20s %8.3f ms (%5.1f%%)\n", "NMS", t.nms_ms, pct(t.nms_ms));
printf(" %-20s %8.3f ms (%5.1f%%)\n", "Double threshold", t.threshold_ms, pct(t.threshold_ms));
printf(" %-20s %8.3f ms (%5.1f%%)\n", "Hysteresis", t.hysteresis_ms,pct(t.hysteresis_ms));
printf(" %-20s %8.3f ms (100.0%%)\n", "TOTAL", t.total_ms);

std::cout << "========================================\n";
std::cout << "\nHotspots (>20%% of total time):\n";

if (pct(t.gaussian_ms) > 20) printf(" -> Gaussian blur %.1f%% — OPTIMIZE WITH RVV\n", pct(t.gaussian_ms));
if (pct(t.sobel_ms) > 20) printf(" -> Sobel %.1f%% — OPTIMIZE WITH RVV\n", pct(t.sobel_ms));
if (pct(t.magnitude_ms) > 20) printf(" -> Magnitude %.1f%% — OPTIMIZE WITH RVV\n", pct(t.magnitude_ms));
    return 0;
}