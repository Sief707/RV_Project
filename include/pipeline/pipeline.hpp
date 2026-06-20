#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "image/image.hpp"
#include <string>
#include <vector>

struct Stage {
    std::string title;
    Image img;
};

struct StageTimings {
    double gaussian_ms;
    double sobel_ms;
    double magnitude1_ms;
    double magnitude2_ms;
    double direction_ms;
    double nms1_ms;
    double nms2_ms;
    double threshold1_ms;
    double threshold2_ms;
    double hysteresis1_ms;
    double hysteresis2_ms;
    double total_1_ms;
    double total_2_ms;
};

struct PipelineResult {
    Image blurred;
    ImageInt32 gx, gy;
    Image magnitude_l1, magnitude_l2;
    Image direction;
    Image thinned_l1, thinned_l2;
    Image thresholded_l1, thresholded_l2;
    Image edges_l1, edges_l2;
    StageTimings timings;
};

void run_pipeline(const Image& input);
StageTimings profile_pipeline(const Image& input);

#endif