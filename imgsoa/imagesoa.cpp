#include "imagesoa.hpp"
#include <vector>
#include <limits>

namespace {
    constexpr int kRedShift = 16;
    constexpr int kGreenShift = 8;
    constexpr uint32_t kMaxByteValue = 0xFF;

    // Function to convert Image (AoS) to SOA
    ImageSOA convertToSOA(const Image& image) {
        ImageSOA soaImage;
        soaImage.width = image.width;
        soaImage.height = image.height;

        // Explicit cast to avoid compiler warnings
        soaImage.maxColorValue = static_cast<uint16_t>(image.max_color_value);

        const size_t numPixels = image.pixels.size();
        soaImage.red.reserve(numPixels);
        soaImage.green.reserve(numPixels);
        soaImage.blue.reserve(numPixels);

        for (const auto& pixel : image.pixels) {
            soaImage.red.push_back(pixel.r);
            soaImage.green.push_back(pixel.g);
            soaImage.blue.push_back(pixel.b);
        }

        return soaImage;
    }
} // namespace

CompressedImage compress_soa(const Image& image) {
    CompressedImage compressedImage;
    compressedImage.width = image.width;
    compressedImage.height = image.height;
    compressedImage.max_color = image.max_color_value;

    const auto soaImage = convertToSOA(image);
    const size_t numPixels = soaImage.red.size();

    // Reserve space for output containers
    compressedImage.pixel_indices.reserve(numPixels);
    compressedImage.color_table.reserve(numPixels / 4); // Assume 25% unique colors

    // Use a vector as a flat lookup table
    constexpr size_t colorRange = 0xFFFFFF + 1; // 24-bit RGB color space
    std::vector<uint32_t> colorToIndex(colorRange, std::numeric_limits<uint32_t>::max());

    for (size_t i = 0; i < numPixels; ++i) {
        const uint32_t packedColor = (static_cast<uint32_t>(soaImage.red[i]) << kRedShift) |
                                     (static_cast<uint32_t>(soaImage.green[i]) << kGreenShift) |
                                     static_cast<uint32_t>(soaImage.blue[i]);

        if (colorToIndex[packedColor] == std::numeric_limits<uint32_t>::max()) {
            // New color, add to color table and update lookup
            colorToIndex[packedColor] = static_cast<uint32_t>(compressedImage.color_table.size());
            compressedImage.color_table.push_back(packedColor);
        }

        // Use the index from the lookup table
        compressedImage.pixel_indices.push_back(colorToIndex[packedColor]);
    }

    return compressedImage;
}

Image decompress_soa(const CompressedImage& compressedImage) {
    Image decompressedImage;
    decompressedImage.width = compressedImage.width;
    decompressedImage.height = compressedImage.height;
    decompressedImage.max_color_value = compressedImage.max_color;

    const size_t totalPixels = compressedImage.pixel_indices.size();
    decompressedImage.pixels.reserve(totalPixels);

    // Build the color table once
    std::vector<Pixel> colorTable(compressedImage.color_table.size());
    for (size_t i = 0; i < compressedImage.color_table.size(); ++i) {
        const uint32_t packedColor = compressedImage.color_table[i];
        colorTable[i] = Pixel{
            .r = static_cast<uint16_t>((packedColor >> kRedShift) & kMaxByteValue),
            .g = static_cast<uint16_t>((packedColor >> kGreenShift) & kMaxByteValue),
            .b = static_cast<uint16_t>(packedColor & kMaxByteValue)
        };
    }

    // Map pixel indices to actual colors
    for (const auto& index : compressedImage.pixel_indices) {
        decompressedImage.pixels.push_back(colorTable[index]);
    }

    return decompressedImage;
}
