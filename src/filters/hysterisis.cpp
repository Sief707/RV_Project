#include "filters/hysterisis.hpp"

#include <queue>
#include <cstdint>

static const uint8_t STRONG = 255;
static const uint8_t WEAK   = 128;

Image hysteresis(const Image& input)
{
    const uint32_t W = input.width;
    const uint32_t H = input.height;

    Image output(W, H);

    // Queue for BFS — stores indices of pixels to visit
    std::queue<uint32_t> to_visit;

    // Step 1: copy all strong pixels to output
    // and add them to the queue as starting points
    for (uint32_t i = 0; i < W * H; ++i)
    {
        if (input.pixels[i] == STRONG)
        {
            output.pixels[i] = STRONG;
            to_visit.push(i);
        }
    }

    // Step 2: BFS flood fill from every strong pixel
    // If a neighbor is weak → promote it to strong and continue
    while (!to_visit.empty())
    {
        const uint32_t idx = to_visit.front();
        to_visit.pop();

        const uint32_t row = idx / W;
        const uint32_t col = idx % W;

        // Check all 8 neighbors
        for (int32_t dr = -1; dr <= 1; ++dr)
        {
            for (int32_t dc = -1; dc <= 1; ++dc)
            {
                // Skip the pixel itself
                if (dr == 0 && dc == 0)
                {
                    continue;
                }

                const int32_t nr = static_cast<int32_t>(row) + dr;
                const int32_t nc = static_cast<int32_t>(col) + dc;

                // Skip if out of bounds
                if (nr < 0 ||
                    nc < 0 ||
                    nr >= static_cast<int32_t>(H) ||
                    nc >= static_cast<int32_t>(W))
                {
                    continue;
                }

                const uint32_t neighbor_idx =
                    static_cast<uint32_t>(nr) * W +
                    static_cast<uint32_t>(nc);

                // If neighbor is weak and not yet visited → promote it
                if (input.pixels[neighbor_idx] == WEAK &&
                    output.pixels[neighbor_idx] != STRONG)
                {
                    output.pixels[neighbor_idx] = STRONG;
                    to_visit.push(neighbor_idx);
                }
            }
        }
    }

    // Step 3: any weak pixel not reached by BFS = noise → stays 0
    // (output was initialized to 0, so nothing to do here)

    return output;
}