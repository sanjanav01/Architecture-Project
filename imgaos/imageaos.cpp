#include <iostream>
#include "imageaos.hpp"
#include <unordered_map>
#include <tuple>
#include <vector>
#include <string>
#include <functional> // For std::hash

// Anonymous namespace for internal linkage
namespace {
    constexpr int kRedShift = 16;
    constexpr int kGreenShift = 8;
    constexpr uint32_t kMaxByteValue = 0xFF;
}

// Custom hash function for std::tuple<uint16_t, uint16_t, uint16_t>
struct TupleHash {
    std::size_t operator()(const std::tuple<uint16_t, uint16_t, uint16_t>& tuple) const {
        const auto redHash = std::hash<uint16_t>()(std::get<0>(tuple));
        const auto greenHash = std::hash<uint16_t>()(std::get<1>(tuple));
        const auto blueHash = std::hash<uint16_t>()(std::get<2>(tuple));
        return redHash ^ (greenHash << 1) ^ (blueHash << 2); // XOR and shift combination
    }
};

CompressedImage compress_aos(const Image& image) {
    CompressedImage compressedImage;
    compressedImage.width = image.width;
    compressedImage.height = image.height;
    compressedImage.max_color = image.max_color_value;

    // Step 1: Create a color table using a hash map to store unique colors
    std::unordered_map<std::tuple<uint16_t, uint16_t, uint16_t>, uint32_t, TupleHash> colorMap;
    std::vector<std::tuple<uint16_t, uint16_t, uint16_t>> colorTable;

    for (const auto& pixel : image.pixels) {
        auto color = std::make_tuple(pixel.r, pixel.g, pixel.b);
        if (!colorMap.contains(color)) {
            colorMap[color] = static_cast<uint32_t>(colorTable.size());
            colorTable.push_back(color);
        }
    }

    // Step 2: Flatten colorTable into compressedImage.color_table
    for (const auto& color : colorTable) {
        compressedImage.color_table.push_back(
            (static_cast<uint32_t>(std::get<0>(color)) << kRedShift) |
            (static_cast<uint32_t>(std::get<1>(color)) << kGreenShift) |
            static_cast<uint32_t>(std::get<2>(color))
        );
    }

    // Step 3: Encode each pixel as an index in the color table
    for (const auto& pixel : image.pixels) {
        auto color = std::make_tuple(pixel.r, pixel.g, pixel.b);
        compressedImage.pixel_indices.push_back(colorMap.at(color));
    }

    return compressedImage;
}

Image decompress(const CompressedImage& compressedImage) {
    Image decompressedImage;
    decompressedImage.width = compressedImage.width;
    decompressedImage.height = compressedImage.height;
    decompressedImage.max_color_value = compressedImage.max_color;

    // Rebuild the color table from the packed color values
    std::vector<std::tuple<uint16_t, uint16_t, uint16_t>> colorTable;
    for (const auto& packedColor : compressedImage.color_table) {
        auto red = static_cast<uint16_t>((packedColor >> kRedShift) & kMaxByteValue);
        auto green = static_cast<uint16_t>((packedColor >> kGreenShift) & kMaxByteValue);
        auto blue = static_cast<uint16_t>(packedColor & kMaxByteValue);
        colorTable.emplace_back(red, green, blue);
    }

    // Decode each pixel using pixel indices and the color table
    for (const auto& index : compressedImage.pixel_indices) {
        const auto& color = colorTable[index];
        const Pixel pixel{.r = std::get<0>(color), .g = std::get<1>(color), .b = std::get<2>(color)};
        decompressedImage.pixels.push_back(pixel);
    }

    return decompressedImage;
}
