#include "helpers.hpp"
#include <cmath>
#include <limits>

// Definition of calculateColorFrequencies
std::map<std::tuple<int, int, int>, int> calculateColorFrequencies(
    const ColorChannels& channels) {
    std::map<std::tuple<int, int, int>, int> color_freq;
    for (size_t i = 0; i < channels.R.size(); ++i) {
        auto color = std::make_tuple(channels.R[i], channels.G[i], channels.B[i]);
        color_freq[color]++;
    }
    return color_freq;
}

// Definition of getInfrequentColors
std::vector<std::tuple<int, int, int>> getInfrequentColors(
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

// Definition of replaceInfrequentColors with ColorChannels struct
void replaceInfrequentColors(
    ColorChannels& channels,
    const std::map<std::tuple<int, int, int>, int>& color_freq,
    int frequency_threshold) {

    for (size_t i = 0; i < channels.R.size(); ++i) {
        auto color = std::make_tuple(channels.R[i], channels.G[i], channels.B[i]);
        if (color_freq.at(color) < frequency_threshold) {
            auto [newR, newG, newB] = findClosestColor(color, color_freq, frequency_threshold);
            channels.R[i] = newR;
            channels.G[i] = newG;
            channels.B[i] = newB;
        }
    }
}

// Definition of findClosestColor
std::tuple<int, int, int> findClosestColor(
    const std::tuple<int, int, int>& color,
    const std::map<std::tuple<int, int, int>, int>& color_freq,
    int frequency_threshold) {

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
