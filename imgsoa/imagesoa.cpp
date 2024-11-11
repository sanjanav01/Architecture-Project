#include "imagesoa.hpp"
#include "helpers/helpers.hpp" // Include the shared helper file
#include <map>
#include <vector>

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

    ImageSOA ImageSOA::resize_soa(const int new_width, const int new_height) const {
    ImageSOA resized_image(new_width, new_height);
    float const x_scale = static_cast<float>(width) / static_cast<float>(new_width);
    float const y_scale = static_cast<float>(height) / static_cast<float>(new_height);
    for (int hgt = 0; hgt < new_height; ++hgt) {
        for (int wdt = 0; wdt < new_width; ++wdt) {
            float const src_x = static_cast<float>(wdt) * x_scale;
            float const src_y = static_cast<float>(hgt) * y_scale;
            int const x_low = static_cast<int>(std::floor(src_x));
            int const x_high = std::min(static_cast<int>(std::ceil(src_x)), width - 1);
            int const y_low = static_cast<int>(std::floor(src_y));
            int const y_high = std::min(static_cast<int>(std::ceil(src_y)), height - 1);
            float const x_weight = src_x - static_cast<float>(x_low);
            float const y_weight = src_y - static_cast<float>(y_low);
            const auto idx_xl_yl = (static_cast<size_t>(y_low) * static_cast<size_t>(width)) + static_cast<size_t>(x_low);
            const auto idx_xh_yl = (static_cast<size_t>(y_low) * static_cast<size_t>(width)) + static_cast<size_t>(x_high);
            const auto idx_xl_yh = (static_cast<size_t>(y_high) * static_cast<size_t>(width)) + static_cast<size_t>(x_low);
            const auto idx_xh_yh = (static_cast<size_t>(y_high) * static_cast<size_t>(width)) + static_cast<size_t>(x_high);
            int const red_val = static_cast<int>(((1.0F - y_weight) * ((1.0F - x_weight) * static_cast<float>(R[idx_xl_yl]) + x_weight * static_cast<float>(R[idx_xh_yl]))) + (y_weight * ((1.0F - x_weight) * static_cast<float>(R[idx_xl_yh]) + x_weight * static_cast<float>(R[idx_xh_yh]))));
            int const green_val = static_cast<int>(((1.0F - y_weight) * ((1.0F - x_weight) * static_cast<float>(G[idx_xl_yl]) + x_weight * static_cast<float>(G[idx_xh_yl]))) + (y_weight * ((1.0F - x_weight) * static_cast<float>(G[idx_xl_yh]) + x_weight * static_cast<float>(G[idx_xh_yh]))));
            int const blue_val = static_cast<int>(((1.0F - y_weight) * ((1.0F - x_weight) * static_cast<float>(B[idx_xl_yl]) + x_weight * static_cast<float>(B[idx_xh_yl]))) + (y_weight * ((1.0F - x_weight) * static_cast<float>(B[idx_xl_yh]) + x_weight * static_cast<float>(B[idx_xh_yh]))));
            resized_image.R[(static_cast<size_t>(hgt) * static_cast<size_t>(new_width)) + static_cast<size_t>(wdt)] = red_val;
            resized_image.G[(static_cast<size_t>(hgt) * static_cast<size_t>(new_width)) + static_cast<size_t>(wdt)] = green_val;
            resized_image.B[(static_cast<size_t>(hgt) * static_cast<size_t>(new_width)) + static_cast<size_t>(wdt)] = blue_val;
        }
    }
    return resized_image;
}

