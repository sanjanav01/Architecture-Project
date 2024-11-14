#include "imageaos.hpp"
#include <unordered_map>
#include <tuple>
#include <vector>

// Anonymous namespace for internal linkage
namespace {
    constexpr int kRedShift = 16;
    constexpr int kGreenShift = 8;
    constexpr uint32_t kMaxByteValue = 0xFF;
}

struct TupleHash {
    std::size_t operator()(const std::tuple<uint16_t, uint16_t, uint16_t>& tuple) const {
        return (static_cast<uint32_t>(std::get<0>(tuple)) << kRedShift) |
               (static_cast<uint32_t>(std::get<1>(tuple)) << kGreenShift) |
               static_cast<uint32_t>(std::get<2>(tuple));
    }
};

CompressedImage compress_aos(const Image& image) {
    CompressedImage compressedImage;
    compressedImage.width = image.width;
    compressedImage.height = image.height;
    compressedImage.max_color = image.max_color_value;

    compressedImage.pixel_indices.reserve(image.pixels.size());

    std::unordered_map<std::tuple<uint16_t, uint16_t, uint16_t>, uint32_t, TupleHash> colorMap;
    colorMap.reserve(image.pixels.size() / 2); // Estimate based on expected unique colors

    for (const auto& pixel : image.pixels) {
        auto color = std::make_tuple(pixel.r, pixel.g, pixel.b);
        if (!colorMap.contains(color)) {
            const uint32_t packedColor = (static_cast<uint32_t>(pixel.r) << kRedShift) |
                                         (static_cast<uint32_t>(pixel.g) << kGreenShift) |
                                         static_cast<uint32_t>(pixel.b);
            compressedImage.color_table.push_back(packedColor);
            colorMap[color] = static_cast<uint32_t>(compressedImage.color_table.size() - 1);
        }
        compressedImage.pixel_indices.push_back(colorMap[color]);
    }

    return compressedImage;
}

Image decompress(const CompressedImage& compressedImage) {
    Image decompressedImage;
    decompressedImage.width = compressedImage.width;
    decompressedImage.height = compressedImage.height;
    decompressedImage.max_color_value = compressedImage.max_color;

  decompressedImage.pixels.reserve(
   static_cast<std::vector<Pixel>::size_type>(
       static_cast<size_t>(compressedImage.width) * static_cast<size_t>(compressedImage.height))
);


    std::vector<std::tuple<uint16_t, uint16_t, uint16_t>> colorTable;
    colorTable.reserve(compressedImage.color_table.size());

    for (const auto& packedColor : compressedImage.color_table) {
        const auto red = static_cast<uint16_t>((packedColor >> kRedShift) & kMaxByteValue);
        const auto green = static_cast<uint16_t>((packedColor >> kGreenShift) & kMaxByteValue);
        const auto blue = static_cast<uint16_t>(packedColor & kMaxByteValue);
        colorTable.emplace_back(red, green, blue);
    }

    for (const auto& index : compressedImage.pixel_indices) {
        const auto& color = colorTable[index];
        decompressedImage.pixels.push_back({std::get<0>(color), std::get<1>(color), std::get<2>(color)});
    }

    return decompressedImage;
}
