#include <cstdint>
#include <fstream>
#include <cmath>
#include <vector>

int main()
{
    const uint32_t W = 512;
    const uint32_t H = 512;

    std::vector<uint8_t> pixels(W * H, 0);

    for (uint32_t y = 0; y < H; ++y)
    {
        for (uint32_t x = 0; x < W; ++x)
        {
            uint8_t value = 0;

            // vertical edge at x = 150
            if (x == 150)
                value = 255;

            // horizontal edge at y = 200
            else if (y == 200)
                value = 255;

            // diagonal edge
            else if (x == y)
                value = 255;

            // white rectangle in center
            else if (x > 200 && x < 350 && y > 200 && y < 350)
                value = 200;

            // circle
            else
            {
                float dx = static_cast<float>(x) - 400.0f;
                float dy = static_cast<float>(y) - 120.0f;
                if (std::sqrt(dx*dx + dy*dy) < 60.0f)
                    value = 180;
            }

            pixels[y * W + x] = value;
        }
    }

    std::ofstream file("images/input.raw", std::ios::binary);
    file.write(reinterpret_cast<const char*>(pixels.data()), pixels.size());

    return 0;
}