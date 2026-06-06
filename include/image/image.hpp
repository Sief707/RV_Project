#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <cstdint>
#include <string>
#include <vector>

class Image
{
public:
    uint32_t width;
    uint32_t height;
    std::vector<uint8_t> pixels;

    Image();
    Image(uint32_t width, uint32_t height);

    bool load_raw(const std::string& filename);
    bool save_raw(const std::string& filename) const;
};

class ImageInt32
{
public:
    uint32_t width;
    uint32_t height;
    std::vector<int32_t> pixels;

    ImageInt32();
    ImageInt32(uint32_t width, uint32_t height);
};

#endif
