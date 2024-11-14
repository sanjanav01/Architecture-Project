#include "imageaos.hpp"
#include "helpers/helpers.hpp" // Include the shared helper file
#include <map>
#include <vector>
#include "common/image_types.hpp"
#include <cmath>
#include <stdexcept>
#include <algorithm> // for std::clamp

// constexpr static int MaxByteValue = 255;
 constexpr static int PixelResizeThreshold = 256;
 constexpr static int BytesPerPixel3 = 3;
 constexpr static int BytesPerPixel6 = 6;

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

// Constructor with width and height parameters
ImageAOS::ImageAOS(int width, int height)
    : pixels(static_cast<size_t>(width * height)), width(width), height(height) {}

// Main cutfreq function, which uses shared helper functions for color analysis
void ImageAOS::cutfreq(int frequency_threshold) {
  // Extract Red, Green, and Blue channels from pixels
  ColorChannels channels;
  for (const auto& pixel : pixels) {
    channels.R.push_back(pixel.r);
    channels.G.push_back(pixel.g);
    channels.B.push_back(pixel.b);
  }

  auto color_freq = calculateColorFrequencies(channels);
  auto infrequent_colors = getInfrequentColors(color_freq, frequency_threshold);
  replaceInfrequentColors(channels, color_freq, frequency_threshold);

  // Update the pixels with new color values
  for (size_t i = 0; i < pixels.size(); ++i) {
    pixels[i].r = static_cast<uint16_t>(channels.R[i]);
    pixels[i].g = static_cast<uint16_t>(channels.G[i]);
    pixels[i].b = static_cast<uint16_t>(channels.B[i]);
  }
}


void ImageAOS::maxlevel(Image& image, int new_max_value) {
    if (new_max_value <= 0) {
        throw std::invalid_argument("New max value must be positive.");
    }
    // Pre-compute scaling factor
    double const scaling_factor = static_cast<double>(new_max_value) / image.max_color_value;
    // Scale each pixel's RGB values using floor instead of round
    for (auto& pixel : image.pixels) {
        pixel.r = static_cast<uint16_t>(std::floor(static_cast<double>(pixel.r) * scaling_factor));
        pixel.g = static_cast<uint16_t>(std::floor(static_cast<double>(pixel.g) * scaling_factor));
        pixel.b = static_cast<uint16_t>(std::floor(static_cast<double>(pixel.b) * scaling_factor));

        // Optional: clamp each channel's value if desired to ensure it is within the [0, new_max_value] range
        pixel.r = std::clamp(pixel.r, static_cast<uint16_t>(0), static_cast<uint16_t>(new_max_value));
        pixel.g = std::clamp(pixel.g, static_cast<uint16_t>(0), static_cast<uint16_t>(new_max_value));
        pixel.b = std::clamp(pixel.b, static_cast<uint16_t>(0), static_cast<uint16_t>(new_max_value));
    }

    // Update the max_color_value in the image
    image.max_color_value = new_max_value;

    // Adjust internal storage based on pixel size requirement
    const size_t total_pixels = static_cast<size_t>(image.width) * static_cast<size_t>(image.height);
    const size_t required_size = (new_max_value >= PixelResizeThreshold)
                                 ? total_pixels * BytesPerPixel6
                                 : total_pixels * BytesPerPixel3;

    // Resize the pixel vector if required
    if (image.pixels.size() * sizeof(Pixel) < required_size) {
        image.pixels.resize(required_size / sizeof(Pixel));
    }
}

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
