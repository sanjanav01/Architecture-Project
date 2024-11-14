#include "imagesoa.hpp"
#include "common/image_types.hpp" // Assuming this contains the definition for Image and Pixel
#include <unordered_map>
#include <tuple>
#include <vector>

namespace {
    // Constants for bit shifts and max byte value
    constexpr int kRedShift = 16;
    constexpr int kGreenShift = 8;
    constexpr int kMaxByteValue = 0xFF;

    // Custom hash function for std::tuple<uint16_t, uint16_t, uint16_t>
    struct TupleHash {
        std::size_t operator()(const std::tuple<uint16_t, uint16_t, uint16_t>& tuple) const {
            return (static_cast<uint32_t>(std::get<0>(tuple)) << kRedShift) |
                   (static_cast<uint32_t>(std::get<1>(tuple)) << kGreenShift) |
                   static_cast<uint32_t>(std::get<2>(tuple));
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
                          std::vector<uint32_t>& colorTableOutput) {
        colorMap.reserve(soaImage.red.size() / 2); // Estimate around 50% unique colors for large images

        for (size_t i = 0; i < soaImage.red.size(); ++i) {
            auto color = std::make_tuple(soaImage.red[i], soaImage.green[i], soaImage.blue[i]);
            if (!colorMap.contains(color)) {
                const uint32_t packedColor = (static_cast<uint32_t>(soaImage.red[i]) << kRedShift) |
                                       (static_cast<uint32_t>(soaImage.green[i]) << kGreenShift) |
                                       static_cast<uint32_t>(soaImage.blue[i]);
                colorMap[color] = static_cast<uint32_t>(colorTableOutput.size());
                colorTableOutput.push_back(packedColor);
            }
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
    compressedImage.color_table.reserve(soaImage.red.size() / 2); // Reserve space based on estimated unique colors
    createColorTable(soaImage, colorMap, compressedImage.color_table);

    // Step 3: Encode each pixel as an index in the color table
    compressedImage.pixel_indices.reserve(soaImage.red.size());
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

    // Reserve space for decompressed pixels
    decompressedImage.pixels.reserve(
        static_cast<std::vector<Pixel>::size_type>(
            static_cast<size_t>(compressedImage.width) * static_cast<size_t>(compressedImage.height))
    );

    // Rebuild the color table from the packed color values
    std::vector<std::tuple<uint16_t, uint16_t, uint16_t>> colorTable;
    colorTable.reserve(compressedImage.color_table.size());
    for (const auto& packedColor : compressedImage.color_table) {
        const auto red = static_cast<uint16_t>((packedColor >> kRedShift) & kMaxByteValue);
        const auto green = static_cast<uint16_t>((packedColor >> kGreenShift) & kMaxByteValue);
        const auto blue = static_cast<uint16_t>(packedColor & kMaxByteValue);
        colorTable.emplace_back(red, green, blue);
    }

    // Decode each pixel using pixel indices and the color table
    for (const auto& index : compressedImage.pixel_indices) {
        const auto& color = colorTable[index];
        decompressedImage.pixels.push_back({std::get<0>(color), std::get<1>(color), std::get<2>(color)});
    }

    return decompressedImage;
}
