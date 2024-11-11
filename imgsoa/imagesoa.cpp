// imagesoa.cpp

#include "imagesoa.hpp"
#include <cmath>
#include <map>
#include <limits>

ImageSOA::ImageSOA(int width, int height) :R(static_cast<size_t>(width * height)), G(static_cast<size_t>(width * height)), B(static_cast<size_t>(width * height)),  width(width), height(height) {}

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
void ImageSOA::replaceInfrequentColors(
    const std::map<std::tuple<int, int, int>, int>& color_freq,
    const std::vector<std::tuple<int, int, int>>& infrequent_colors,
    int frequency_threshold) {

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
    int frequency_threshold) {

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
