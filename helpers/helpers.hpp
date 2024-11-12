#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <map>
#include <tuple>
#include <vector>

// Struct to encapsulate R, G, and B channels
struct ColorChannels {
    std::vector<int> R;
    std::vector<int> G;
    std::vector<int> B;
};

// Helper function declarations
std::map<std::tuple<int, int, int>, int> calculateColorFrequencies(
    const ColorChannels& channels);

std::vector<std::tuple<int, int, int>> getInfrequentColors(
    const std::map<std::tuple<int, int, int>, int>& color_freq,
    int frequency_threshold);

void replaceInfrequentColors(
    ColorChannels& channels,
    const std::map<std::tuple<int, int, int>, int>& color_freq,
    int frequency_threshold);

std::tuple<int, int, int> findClosestColor(
    const std::tuple<int, int, int>& color,
    const std::map<std::tuple<int, int, int>, int>& color_freq,
    int frequency_threshold);

#endif // HELPERS_HPP
