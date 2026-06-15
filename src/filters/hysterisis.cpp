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


    // Queue for Breadth First Search (BFS) — stores indices of pixels to visit
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

    // Step 2: BFS Queue is filled from every strong pixel
    // If a neighbor is weak → promote it to strong and continue
    while (!to_visit.empty())
    {
        const uint32_t idx = to_visit.front();
        to_visit.pop();


        // convert 1D indices to 2D coordinates
        const uint32_t row = idx / W;
        const uint32_t col = idx % W;

        // Check all 8 neighbors
        for (int32_t delta_row = -1; delta_row <= 1; ++delta_row)
        {
            for (int32_t delta_column = -1; delta_column <= 1; ++delta_column)
            {
                // Skip the pixel itself
                if (delta_row == 0 && delta_column == 0)
                {
                    continue;
                }


                const int32_t neighbor_row = static_cast<int32_t>(row) + delta_row;
                const int32_t neighbor_column = static_cast<int32_t>(col) + delta_column;

                // Skip if out of bounds
                if (neighbor_row < 0 ||
                    neighbor_column < 0 ||
                    neighbor_row >= static_cast<int32_t>(H) ||
                    neighbor_column >= static_cast<int32_t>(W))
                {
                    continue;
                }

                const uint32_t neighbor_idx =
                static_cast<uint32_t>(neighbor_row) * W + static_cast<uint32_t>(neighbor_column);

                // If neighbor is weak and not yet visited → promote it
                // If pixel X is WEAK in the input, and it gets reached as a neighbor of some strong/promoted pixel, 
                // sets output[X] = STRONG — even though input[X] is still WEAK 
                // (the input array never changes — we only ever write to output).
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