// imageaos.cpp

#include "imageaos.hpp"
#include <cmath>
#include <map>
#include <limits>

ImageAOS::ImageAOS(int w, int h) : pixels(static_cast<size_t>(w * h)), width(w), height(h) {}

// Main cutfreq function, split into helper functions for clarity and compliance
void ImageAOS::cutfreq(int frequency_threshold) {
    auto color_freq = calculateColorFrequencies();
    auto infrequent_colors = getInfrequentColors(color_freq, frequency_threshold);
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

// Helper function to identify infrequent colors
std::vector<std::tuple<int, int, int>> ImageAOS::getInfrequentColors(
    const std::map<std::tuple<int, int, int>, int>& color_freq,
    int frequency_threshold) const {
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
    int frequency_threshold) {

    (void)infrequent_colors;  // Suppress unused parameter warning

    for (auto& pixel : pixels) {
        auto color = std::make_tuple(pixel.R, pixel.G, pixel.B);
        if (color_freq.at(color) < frequency_threshold) {
            auto [newR, newG, newB] = findClosestColor(color, color_freq, frequency_threshold);
            pixel.R = newR;
            pixel.G = newG;
            pixel.B = newB;
        }
    }
}

// Helper function to find the closest replacement color
std::tuple<int, int, int> ImageAOS::findClosestColor(
    const std::tuple<int, int, int>& color,
    const std::map<std::tuple<int, int, int>, int>& color_freq,
    int frequency_threshold) const {

    double min_distance = std::numeric_limits<double>::max();
    std::tuple<int, int, int> closest_color;

    for (const auto& [frequent_color, freq] : color_freq) {
        if (freq >= frequency_threshold) {
            double distance = std::sqrt(
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
