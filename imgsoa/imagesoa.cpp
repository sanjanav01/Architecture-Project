// imagesoa.cpp

#include "imagesoa.hpp"
#include <cmath>
#include <map>
#include <limits>

ImageSOA::ImageSOA(const int width, const int height) :R(static_cast<size_t>(width * height)), G(static_cast<size_t>(width * height)), B(static_cast<size_t>(width * height)),  width(width), height(height) {}

// Main cutfreq function, split into helper functions for clarity and compliance
void ImageSOA::cutfreq(int frequency_threshold) {
    auto color_freq = calculateColorFrequencies();
    auto infrequent_colors = getInfrequentColors(color_freq, frequency_threshold);
    replaceInfrequentColors(color_freq, infrequent_colors, frequency_threshold);
}

// Helper function to calculate color frequencies
std::map<std::tuple<int, int, int>, int> ImageSOA::calculateColorFrequencies() const {
    std::map<std::tuple<int, int, int>, int> color_freq;
    for (size_t i = 0; i < R.size(); ++i) {
        auto color = std::make_tuple(R[i], G[i], B[i]);
        color_freq[color]++;
    }
    return color_freq;
}

// Helper function to identify infrequent colors
std::vector<std::tuple<int, int, int>> ImageSOA::getInfrequentColors(
    const std::map<std::tuple<int, int, int>, int>& color_freq,
    const int frequency_threshold) {
    std::vector<std::tuple<int, int, int>> infrequent_colors;
    for (const auto& [color, freq] : color_freq) {
        if (freq < frequency_threshold) {
            infrequent_colors.push_back(color);
        }
    }
    return infrequent_colors;
}

// Helper function to replace infrequent colors
void ImageSOA::replaceInfrequentColors(
    const std::map<std::tuple<int, int, int>, int>& color_freq,
    const std::vector<std::tuple<int, int, int>>& infrequent_colors,
    const int frequency_threshold) {

    (void)infrequent_colors;  // Suppress unused parameter warning

    for (size_t i = 0; i < R.size(); ++i) {
        auto color = std::make_tuple(R[i], G[i], B[i]);
        if (color_freq.at(color) < frequency_threshold) {
            auto [newR, newG, newB] = findClosestColor(color, color_freq, frequency_threshold);
            R[i] = newR;
            G[i] = newG;
            B[i] = newB;
        }
    }
}

// Helper function to find the closest replacement color
std::tuple<int, int, int> ImageSOA::findClosestColor(
    const std::tuple<int, int, int>& color,
    const std::map<std::tuple<int, int, int>, int>& color_freq,
    const int frequency_threshold) {

    double min_distance = std::numeric_limits<double>::max();
    std::tuple<int, int, int> closest_color;

    for (const auto& [frequent_color, freq] : color_freq) {
        if (freq >= frequency_threshold) {
            double const distance = std::sqrt(
                std::pow(std::get<0>(color) - std::get<0>(frequent_color), 2) +
                std::pow(std::get<1>(color) - std::get<1>(frequent_color), 2) +
                std::pow(std::get<2>(color) - std::get<2>(frequent_color), 2)
            );

            if (distance < min_distance) {
                min_distance = distance;
                closest_color = frequent_color;
            }
        }
    }

    return closest_color;
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

