#include "image/image.hpp"

#include <fstream>

Image::Image()
    : width(0),
      height(0)
{
}

Image::Image(uint32_t width, uint32_t height)
    : width(width),
      height(height),
      pixels(width * height)
{
}

bool Image::load_raw(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary);

    if (!file)
    {
        return false;
    }

    file.read(reinterpret_cast<char*>(pixels.data()),
              pixels.size());

    return file.good() || file.eof();
}

bool Image::save_raw(const std::string& filename) const
{
    std::ofstream file(filename, std::ios::binary);

    if (!file)
    {
        return false;
    }

    file.write(reinterpret_cast<const char*>(pixels.data()),
               pixels.size());

    return file.good();
}

ImageInt32::ImageInt32()
    : width(0),
      height(0)
{
}

ImageInt32::ImageInt32(uint32_t width, uint32_t height)
    : width(width),
      height(height),
      pixels(width * height)
{
}
