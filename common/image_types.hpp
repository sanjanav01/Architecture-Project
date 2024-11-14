#ifndef IMAGE_TYPES_HPP
#define IMAGE_TYPES_HPP

#include <cstddef>
#include <vector>
#include <cstdint>
constexpr static int MAGICNUMB = 255;
struct Pixel {
    uint16_t r, g, b;
};

class Image {
public:
    Image() = default;  // Default constructor
    Image(int width, int height) : width(width), height(height), pixels(static_cast<std::vector<Pixel>::size_type>(width * height)) {}

    int width = 0;
    int height = 0;
    int max_color_value = MAGICNUMB;
    std::vector<Pixel> pixels;
};


struct CompressedImage {
    int width = 0;
    int height = 0;
    int max_color = 0;
    std::vector<uint32_t> color_table; // Stores unique colors in the image
    std::vector<uint32_t> pixel_indices; // Compressed pixel data as indices to color_table
};

#endif