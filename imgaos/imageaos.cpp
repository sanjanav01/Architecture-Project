#include "imageaos.hpp"
#include <unordered_map>
#include <vector>

namespace {
    constexpr int kRedShift = 16;
    constexpr int kGreenShift = 8;
    constexpr uint32_t kMaxByteValue = 0xFF;
} // namespace

CompressedImage compress_aos(const Image& image) {
    CompressedImage compressedImage;
    compressedImage.width = image.width;
    compressedImage.height = image.height;
    compressedImage.max_color = image.max_color_value;

    const size_t numPixels = image.pixels.size();
    compressedImage.pixel_indices.reserve(numPixels);

    std::unordered_map<uint32_t, uint32_t> colorMap; // Map packed color to index
    colorMap.reserve(numPixels / 4);

    for (const auto& pixel : image.pixels) {
        const uint32_t packedColor = (static_cast<uint32_t>(pixel.r) << kRedShift) |
                               (static_cast<uint32_t>(pixel.g) << kGreenShift) |
                               static_cast<uint32_t>(pixel.b);
        auto [it, inserted] = colorMap.emplace(packedColor, colorMap.size());
        if (inserted) {
            compressedImage.color_table.push_back(packedColor);
        }
        compressedImage.pixel_indices.push_back(it->second);
    }

    return compressedImage;
}

Image decompress(const CompressedImage& compressedImage) {
    Image decompressedImage;
    decompressedImage.width = compressedImage.width;
    decompressedImage.height = compressedImage.height;
    decompressedImage.max_color_value = compressedImage.max_color;

    const size_t totalPixels = compressedImage.pixel_indices.size();
    decompressedImage.pixels.reserve(totalPixels);

    std::vector<Pixel> colorTable;
    colorTable.reserve(compressedImage.color_table.size());
    for (const auto& packedColor : compressedImage.color_table) {
        colorTable.push_back({
            static_cast<uint16_t>((packedColor >> kRedShift) & kMaxByteValue),
            static_cast<uint16_t>((packedColor >> kGreenShift) & kMaxByteValue),
            static_cast<uint16_t>(packedColor & kMaxByteValue)
        });
    }

    for (const auto& index : compressedImage.pixel_indices) {
        decompressedImage.pixels.push_back(colorTable[index]);
    }

    return decompressedImage;
}
