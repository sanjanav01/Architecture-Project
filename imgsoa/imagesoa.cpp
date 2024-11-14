#include "imagesoa.hpp"
#include "common/image_types.hpp" // Assuming this contains the definition for Image and Pixel
#include <unordered_map>
#include <tuple>
#include <vector>
#include <functional> // For std::hash

namespace {
    // Constants for bit shifts and max byte value
    constexpr int kRedShift = 16;
    constexpr int kGreenShift = 8;
    constexpr int kMaxByteValue = 0xFF;

    // Custom hash function for std::tuple<uint16_t, uint16_t, uint16_t>
    struct TupleHash {
        std::size_t operator()(const std::tuple<uint16_t, uint16_t, uint16_t>& tuple) const {
            const auto redHash = std::hash<uint16_t>()(std::get<0>(tuple));
            const auto greenHash = std::hash<uint16_t>()(std::get<1>(tuple));
            const auto blueHash = std::hash<uint16_t>()(std::get<2>(tuple));
            return redHash ^ (greenHash << 1) ^ (blueHash << 2); // XOR and shift combination
        }
    };

    // Helper to convert Image (AoS) to ImageSOA
    ImageSOA convertToSOA(const Image& image) {
        ImageSOA soaImage;
        soaImage.width = image.width;
        soaImage.height = image.height;
        soaImage.maxColorValue = static_cast<uint16_t>(image.max_color_value);

        soaImage.red.reserve(image.pixels.size());
        soaImage.green.reserve(image.pixels.size());
        soaImage.blue.reserve(image.pixels.size());

        for (const auto& pixel : image.pixels) {
            soaImage.red.push_back(pixel.r);
            soaImage.green.push_back(pixel.g);
            soaImage.blue.push_back(pixel.b);
        }

        return soaImage;
    }

    // Helper to create color table and map unique colors to indices
    void createColorTable(const ImageSOA& soaImage,
                          std::unordered_map<std::tuple<uint16_t, uint16_t, uint16_t>, uint32_t, TupleHash>& colorMap,
                          std::vector<std::tuple<uint16_t, uint16_t, uint16_t>>& colorTable) {
        for (size_t i = 0; i < soaImage.red.size(); ++i) {
            auto color = std::make_tuple(soaImage.red[i], soaImage.green[i], soaImage.blue[i]);
            if (!colorMap.contains(color)) {
                colorMap[color] = static_cast<uint32_t>(colorTable.size());
                colorTable.push_back(color);
            }
        }
    }

    // Helper to flatten color table into compressed image's color_table
    void flattenColorTable(const std::vector<std::tuple<uint16_t, uint16_t, uint16_t>>& colorTable,
                           std::vector<uint32_t>& colorTableOutput) {
        for (const auto& color : colorTable) {
            colorTableOutput.push_back(
                (static_cast<uint32_t>(std::get<0>(color)) << kRedShift) |
                (static_cast<uint32_t>(std::get<1>(color)) << kGreenShift) |
                static_cast<uint32_t>(std::get<2>(color))
            );
        }
    }
}

// Main compress function using SOA structure
CompressedImage compress_soa(const Image& image) {
    CompressedImage compressedImage;
    compressedImage.width = image.width;
    compressedImage.height = image.height;
    compressedImage.max_color = image.max_color_value;

    // Step 1: Convert Image (AoS) to SoA format
    const auto soaImage = convertToSOA(image);

    // Step 2: Create a color table using a hash map to store unique colors
    std::unordered_map<std::tuple<uint16_t, uint16_t, uint16_t>, uint32_t, TupleHash> colorMap;
    std::vector<std::tuple<uint16_t, uint16_t, uint16_t>> colorTable;
    createColorTable(soaImage, colorMap, colorTable);

    // Step 3: Flatten color table into compressedImage.color_table
    flattenColorTable(colorTable, compressedImage.color_table);

    // Step 4: Encode each pixel as an index in the color table
    for (size_t i = 0; i < soaImage.red.size(); ++i) {
        auto color = std::make_tuple(soaImage.red[i], soaImage.green[i], soaImage.blue[i]);
        compressedImage.pixel_indices.push_back(colorMap.at(color));
    }

    return compressedImage;
}

// Decompression function to reconstruct the image from compressed data
Image decompress_soa(const CompressedImage& compressedImage) {
    Image decompressedImage;
    decompressedImage.width = compressedImage.width;
    decompressedImage.height = compressedImage.height;
    decompressedImage.max_color_value = compressedImage.max_color;

    // Rebuild the color table from the packed color values
    std::vector<std::tuple<uint16_t, uint16_t, uint16_t>> colorTable;
    for (const auto& packedColor : compressedImage.color_table) {
        const auto red = static_cast<uint16_t>((packedColor >> kRedShift) & kMaxByteValue);
        const auto green = static_cast<uint16_t>((packedColor >> kGreenShift) & kMaxByteValue);
        const auto blue = static_cast<uint16_t>(packedColor & kMaxByteValue);
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
