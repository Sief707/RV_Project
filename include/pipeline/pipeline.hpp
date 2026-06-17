#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "image/image.hpp"

struct Stage {
std::string title;
Image img;
};

static void save_ppm(const std::string& filename, const Image& img);
static void print_stats(const std::string& label, const Image& img);

static void print_comparison_report(
const Image& mag_l1, const Image& mag_l2,
const Image& thin_l1, const Image& thin_l2,
const Image& thresh_l1, const Image& thresh_l2,
const Image& edges_l1, const Image& edges_l2);

static void save_grid_ppm(const std::string& filename,
const std::vector<Stage>& stages,
const std::string& pipeline_title);

void run_pipeline(const Image& input);

#endif