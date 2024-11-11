// imageaos.cpp

#include "imageaos.hpp"
#include <cmath>
#include <map>
#include <limits>

// Constructor with width and height parameters
ImageAOS::ImageAOS(const int width, const int height)
    : pixels(static_cast<size_t>(width * height)), width(width), height(height) {}

// Main cutfreq function, split into helper functions for clarity and compliance
void ImageAOS::cutfreq(const int frequency_threshold) {
    const auto color_freq = calculateColorFrequencies();
    const auto infrequent_colors = getInfrequentColors(color_freq, frequency_threshold);
    replaceInfrequentColors(color_freq, infrequent_colors, frequency_threshold);
}

// Helper function to calculate color frequencies
std::map<std::tuple<int, int, int>, int> ImageAOS::calculateColorFrequencies() const {
    std::map<std::tuple<int, int, int>, int> color_freq;
    for (const auto& pixel : pixels) {
        auto color = std::make_tuple(pixel.R, pixel.G, pixel.B);
        color_freq[color]++;
    }
    return color_freq;
}

// Static helper function to identify infrequent colors
std::vector<std::tuple<int, int, int>> ImageAOS::getInfrequentColors(
    const std::map<std::tuple<int, int, int>, int>& color_freq,
    int frequency_threshold) {
    std::vector<std::tuple<int, int, int>> infrequent_colors;
    for (const auto& [color, freq] : color_freq) {
        if (freq < frequency_threshold) {
            infrequent_colors.push_back(color);
        }
    }
    return infrequent_colors;
}

// Helper function to replace infrequent colors
void ImageAOS::replaceInfrequentColors(
    const std::map<std::tuple<int, int, int>, int>& color_freq,
    const std::vector<std::tuple<int, int, int>>& infrequent_colors,
    const int frequency_threshold) {

    (void)infrequent_colors;  // Suppress unused parameter warning

    for (auto&[R, G, B] : pixels) {
        if (auto color = std::make_tuple(R, G, B); color_freq.at(color) < frequency_threshold) {
            auto [newR, newG, newB] = findClosestColor(color, color_freq, frequency_threshold);
            R = newR;
            G = newG;
            B = newB;
        }
    }
}

// Static helper function to find the closest replacement color
std::tuple<int, int, int> ImageAOS::findClosestColor(
    const std::tuple<int, int, int>& color,
    const std::map<std::tuple<int, int, int>, int>& color_freq,
    const int frequency_threshold) {

    double min_distance = std::numeric_limits<double>::max();
    std::tuple<int, int, int> closest_color;

    for (const auto& [frequent_color, freq] : color_freq) {
        if (freq >= frequency_threshold) {
            const double distance = std::sqrt(
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

ImageAOS resize_aos(const ImageAOS& image, const int new_width, const int new_height) {
    ImageAOS resized_image(new_width, new_height);

    // Scaling factors for width and height
    float const x_scale = static_cast<float>(image.width) / static_cast<float>(new_width);
    float const y_scale = static_cast<float>(image.height) / static_cast<float>(new_height);

    // Nearest-neighbor interpolation loop
    for (int hgt = 0; hgt < new_height; ++hgt) {
        for (int wdt = 0; wdt < new_width; ++wdt) {
            // Calculate the nearest source coordinates in the original image
            auto src_x = static_cast<size_t>(std::round(static_cast<float>(hgt) * x_scale));
            auto src_y = static_cast<size_t>(std::round(static_cast<float>(wdt) * y_scale));
            src_x = std::min(src_x, static_cast<size_t>(image.width - 1));
            src_y = std::min(src_y, static_cast<size_t>(image.height - 1));

            // Set the pixel in the resized image
            resized_image.pixels[(static_cast<size_t>(hgt) * static_cast<size_t>(new_width)) + static_cast<size_t>(wdt)] =
                image.pixels[(src_y * static_cast<size_t>(image.width)) + src_x];
        }
    }
    return resized_image;
}
