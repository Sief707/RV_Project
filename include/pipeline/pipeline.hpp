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
    double magnitude_ms;
    double direction_ms;
    double nms_ms;
    double threshold_ms;
    double hysteresis_ms;
    double total_ms;
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