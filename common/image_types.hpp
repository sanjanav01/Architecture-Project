#ifndef IMAGE_TYPES_HPP
#define IMAGE_TYPES_HPP

#include <vector>
#include <cstdint>

struct Pixel {
    uint16_t r, g, b;
};

struct Image {
    int width = 0;
    int height = 0;
    int max_color_value = 0;
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
