#include "imagesoa.hpp"
#include "helpers/helpers.hpp" // Include the shared helper file
#include <map>
#include <vector>
#include <algorithm>

// Constructor with width and height parameters
ImageSOA::ImageSOA(const int width, int const height)
    : R(static_cast<size_t>(width * height)), G(static_cast<size_t>(width * height)),
      B(static_cast<size_t>(width * height)), width(width), height(height) {}

// Main cutfreq function, which uses shared helper functions for color analysis
void ImageSOA::cutfreq(int frequency_threshold) {
    // Create a ColorChannels instance to group R, G, and B channels
    ColorChannels channels = {.R=R, .G=G, .B=B};

    auto color_freq = calculateColorFrequencies(channels);
    auto infrequent_colors = getInfrequentColors(color_freq, frequency_threshold);
    replaceInfrequentColors(channels, color_freq, frequency_threshold);
}

ImageSOA resize_soa(const ImageSOA& image, const int new_width, const int new_height) {
    ImageSOA resized_image(new_width, new_height);
    const float x_scale = static_cast<float>(image.width - 1) / static_cast<float>(new_width - 1);
    const float y_scale = static_cast<float>(image.height - 1) / static_cast<float>(new_height - 1);
    for (int y_new = 0; y_new < new_height; ++y_new) {
        const float src_y = static_cast<float>(y_new) * y_scale;
        const int yfloor = static_cast<int>(src_y);
        const int yfloor1 = std::min(yfloor + 1, image.height - 1);
        const float y_frac = src_y - static_cast<float>(yfloor);
        for (int x_new = 0; x_new < new_width; ++x_new) {
            const float src_x = static_cast<float>(x_new) * x_scale;
            const int xfloor = static_cast<int>(src_x);
            const int xfloor1 = std::min(xfloor + 1, image.width - 1);
            const float x_frac = src_x - static_cast<float>(xfloor);
            const size_t index00 = (static_cast<size_t>(yfloor) * static_cast<size_t>(image.width)) + static_cast<size_t>(xfloor);
            const size_t index10 = (static_cast<size_t>(yfloor) * static_cast<size_t>(image.width)) + static_cast<size_t>(xfloor1);
            const size_t index01 = (static_cast<size_t>(yfloor1) * static_cast<size_t>(image.width)) + static_cast<size_t>(xfloor);
            const size_t index11 = (static_cast<size_t>(yfloor1) * static_cast<size_t>(image.width)) + static_cast<size_t>(xfloor1);
            auto getRGB = [&](const size_t index) {return std::make_tuple(image.R.at(index), image.G.at(index), image.B.at(index));};
            const auto [R00, G00, B00] = getRGB(index00);
            const auto [R10, G10, B10] = getRGB(index10);
            const auto [R01, G01, B01] = getRGB(index01);
            const auto [R11, G11, B11] = getRGB(index11);
            const float R_interpolated = ((1.0F - y_frac) * ((1.0F - x_frac) * static_cast<float>(R00) + x_frac * static_cast<float>(R10))) +(y_frac * ((1.0F - x_frac) * static_cast<float>(R01) + x_frac * static_cast<float>(R11)));
            const float G_interpolated = ((1.0F - y_frac) * ((1.0F - x_frac) * static_cast<float>(G00) + x_frac * static_cast<float>(G10))) +(y_frac * ((1.0F - x_frac) * static_cast<float>(G01) + x_frac * static_cast<float>(G11)));
            const float B_interpolated = ((1.0F - y_frac) * ((1.0F - x_frac) * static_cast<float>(B00) + x_frac * static_cast<float>(B10))) +(y_frac * ((1.0F - x_frac) * static_cast<float>(B01) + x_frac * static_cast<float>(B11)));
            const uint8_t R_new = static_cast<uint8_t>(std::clamp(R_interpolated, 0.0F, 255.0F));
            const uint8_t G_new = static_cast<uint8_t>(std::clamp(G_interpolated, 0.0F, 255.0F));
            const uint8_t B_new = static_cast<uint8_t>(std::clamp(B_interpolated, 0.0F, 255.0F));
            const size_t index_new = (static_cast<size_t>(y_new) * static_cast<size_t>(new_width)) + static_cast<size_t>(x_new);
            resized_image.R.at(index_new) = R_new;
            resized_image.G.at(index_new) = G_new;
            resized_image.B.at(index_new) = B_new;
        }
    }
    return resized_image;
}

