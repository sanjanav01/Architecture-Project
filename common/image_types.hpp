#ifndef IMAGE_TYPES_HPP
#define IMAGE_TYPES_HPP

#include <vector>
#include <cstdint>

struct Pixel {
    uint16_t r, g, b;
};

struct Image {
    int width;
    int height;
    int max_color_value;
    std::vector<Pixel> pixels;
};

struct CompressedImage {
    int width;
    int height;
    int max_color;
    std::vector<uint32_t> color_table; // Stores unique colors in the image
    std::vector<uint32_t> pixel_indices; // Compressed pixel data as indices to color_table
};

#endif
