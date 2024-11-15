#include "imagesoa.hpp"
#include "helpers/helpers.hpp" // Include the shared helper file
#include "common/image_types.hpp" // For Image, Pixel, etc.
#include <map>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <tuple>

// Constants for bit shifts and max byte value
constexpr static int kRedShift = 16;
constexpr static int kGreenShift = 8;
constexpr static int kMaxByteValue = 0xFF;
constexpr static int PixelThreshold = 256;
constexpr static int MAX_VAL = 256;


// Constructor with max color value
ImageSOA::ImageSOA(Width const width, Height const height, MaxColorValue const max_color_value)
    : width(width.value),  // Initialize width
      height(height.value), // Initialize height
      current_max_color_value(max_color_value.value), // Initialize max color value
      R(static_cast<size_t>(width.value * height.value)),      // Initialize R
      G(static_cast<size_t>(width.value * height.value)),      // Initialize G
      B(static_cast<size_t>(width.value * height.value))       // Initialize B
{}

// Constructor with default max color value
ImageSOA::ImageSOA(Width const width, Height const height)
    : width(width.value),  // Initialize width
      height(height.value), // Initialize height
      current_max_color_value(MAX_VAL), // Default max color value
      R(static_cast<size_t>(width.value * height.value)),      // Initialize R
      G(static_cast<size_t>(width.value * height.value)),      // Initialize G
      B(static_cast<size_t>(width.value * height.value))       // Initialize B
{}

namespace {

  struct TupleHash {
    std::size_t operator()(const std::tuple<uint16_t, uint16_t, uint16_t>& tuple) const {
      return (static_cast<uint32_t>(std::get<0>(tuple)) << kRedShift) |
             (static_cast<uint32_t>(std::get<1>(tuple)) << kGreenShift) |
             static_cast<uint32_t>(std::get<2>(tuple));
    }
  };

  // Helper: Convert Image (AOS) to ImageSOA
  ImageSOA convertToSOA(const Image& image) {
    ImageSOA soaImage(Width{image.width}, Height{image.height}, MaxColorValue{image.max_color_value});
    for (size_t i = 0; i < image.pixels.size(); ++i) {
      soaImage.R[i] = image.pixels[i].r;
      soaImage.G[i] = image.pixels[i].g;
      soaImage.B[i] = image.pixels[i].b;
    }
    return soaImage;
  }

  // Helper: Create color table and map unique colors to indices
  void createColorTable(const ImageSOA& soaImage,
                          std::unordered_map<std::tuple<uint16_t, uint16_t, uint16_t>, uint32_t, TupleHash>& colorMap,
                          std::vector<uint32_t>& colorTableOutput) {
    colorMap.reserve(soaImage.R.size() / 2); // Estimate around 50% unique colors for large images

    for (size_t i = 0; i < soaImage.R.size(); ++i) {
      auto color = std::make_tuple(soaImage.R[i], soaImage.G[i], soaImage.B[i]);
      if (!colorMap.contains(color)) {
        const uint32_t packedColor = (static_cast<uint32_t>(soaImage.R[i]) << kRedShift) |
                               (static_cast<uint32_t>(soaImage.G[i]) << kGreenShift) |
                               static_cast<uint32_t>(soaImage.B[i]);
        colorMap[color] = static_cast<uint32_t>(colorTableOutput.size());
        colorTableOutput.push_back(packedColor);
      }
    }
  }
}

void ImageSOA::cutfreq(int frequency_threshold) {
  // Create a ColorChannels instance to group R, G, and B channels
  ColorChannels channels = {.R=R, .G=G, .B=B};

  auto color_freq = calculateColorFrequencies(channels);
  auto infrequent_colors = getInfrequentColors(color_freq, frequency_threshold);
  replaceInfrequentColors(channels, color_freq, frequency_threshold);

  R = channels.R;
  G = channels.G;
  B = channels.B;
}

// Main maxlevel function
void ImageSOA::maxlevel(int new_max_value) {
    if (new_max_value <= 0) {
        throw std::invalid_argument("New max value must be positive.");
    }

    // Pre-compute scaling factor
    double const scaling_factor = static_cast<double>(new_max_value) / current_max_color_value;

    // Scale each RGB component independently
    for (size_t i = 0; i < R.size(); ++i) {
        R[i] = static_cast<int>(std::floor(R[i] * scaling_factor));
        G[i] = static_cast<int>(std::floor(G[i] * scaling_factor));
        B[i] = static_cast<int>(std::floor(B[i] * scaling_factor));

        // Clamp each channel to ensure it is within [0, new_max_value]
        R[i] = std::clamp(R[i], 0, new_max_value);
        G[i] = std::clamp(G[i], 0, new_max_value);
        B[i] = std::clamp(B[i], 0, new_max_value);
    }

    // Update the current max color value to the new max value
    current_max_color_value = new_max_value;

    // Inform output handling about byte representation
    size_t const total_pixels = static_cast<size_t>(width) * static_cast<size_t>(height);
    if (current_max_color_value < PixelThreshold) {
        if (R.size() < total_pixels || G.size() < total_pixels || B.size() < total_pixels) {
            R.resize(total_pixels);
            G.resize(total_pixels);
            B.resize(total_pixels);
        }
        std::cout << "Switching to 3-byte representation (max color value < 256).\n";
    } else {
        if (R.size() < 2 * total_pixels || G.size() < 2 * total_pixels || B.size() < 2 * total_pixels) {
            R.resize(2 * total_pixels);
            G.resize(2 * total_pixels);
            B.resize(2 * total_pixels);
        }
        std::cout << "Switching to 6-byte representation (max color value >= 256).\n";
    }
}

// Compression function for ImageSOA
CompressedImage compress_soa(const Image& image) {
  CompressedImage compressedImage;
  compressedImage.width = image.width;
  compressedImage.height = image.height;
  compressedImage.max_color = image.max_color_value;

  // Step 1: Convert Image (AoS) to SoA format
  const auto soaImage = convertToSOA(image);

  // Step 2: Create a color table using a hash map to store unique colors
  std::unordered_map<std::tuple<uint16_t, uint16_t, uint16_t>, uint32_t, TupleHash> colorMap;
  compressedImage.color_table.reserve(soaImage.R.size() / 2); // Reserve space based on estimated unique colors
  createColorTable(soaImage, colorMap, compressedImage.color_table);

  // Step 3: Encode each pixel as an index in the color table
  compressedImage.pixel_indices.reserve(soaImage.R.size());
  for (size_t i = 0; i < soaImage.R.size(); ++i) {
    auto color = std::make_tuple(soaImage.R[i], soaImage.G[i], soaImage.B[i]);
    compressedImage.pixel_indices.push_back(colorMap.at(color));
  }

  return compressedImage;
}

// Decompression function for ImageSOA
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

struct InterpolationParams {
    int c00, c10, c01, c11;
    float x_frac, y_frac;
};

ImageSOA ImageSOA::resize_soa(const int new_width, const ImageSOA& input_soa, int new_height) {
    Width const width = {new_width};
    Height const height = {new_height};
    ImageSOA resized_soa(width, height);
    resized_soa.R.clear();
    resized_soa.G.clear();
    resized_soa.B.clear();
    float const x_scale = static_cast<float>(input_soa.width - 1) / static_cast<float>(new_width - 1);
    float const y_scale = static_cast<float>(input_soa.height - 1) / static_cast<float>(new_height - 1);
    auto interpolate = [](const InterpolationParams& params) -> int {
        float const top = ((1.0F - params.x_frac) * static_cast<float>(params.c00)) +(params.x_frac * static_cast<float>(params.c10));
        float const bottom = ((1.0F - params.x_frac) * static_cast<float>(params.c01)) +(params.x_frac * static_cast<float>(params.c11));
        return static_cast<int>(std::round(((1.0F - params.y_frac) * top) + (params.y_frac * bottom)));};
    for (int y_new = 0; y_new < new_height; ++y_new) {
        float const y_pos = static_cast<float>(y_new) * y_scale;
        int const y_floor = static_cast<int>(y_pos);
        int const y_ceil = std::min(y_floor + 1, input_soa.height - 1);
        float const y_frac = y_pos - static_cast<float>(y_floor);
        for (int x_new = 0; x_new < new_width; ++x_new) {
            float const x_pos = static_cast<float>(x_new) * x_scale;
            int const x_floor = static_cast<int>(x_pos);
            int const x_ceil = std::min(x_floor + 1, input_soa.width - 1);
            float const x_frac = x_pos - static_cast<float>(x_floor);
            size_t const idx_yfloor_xfloor = (static_cast<size_t>(y_floor) * static_cast<size_t>(input_soa.width)) + static_cast<size_t>(x_floor);
            size_t const idx_yfloor_xceil = (static_cast<size_t>(y_floor) * static_cast<size_t>(input_soa.width)) + static_cast<size_t>(x_ceil);
            size_t const idx_yceil_xfloor = (static_cast<size_t>(y_ceil) * static_cast<size_t>(input_soa.width)) + static_cast<size_t>(x_floor);
            size_t const idx_yceil_xceil = (static_cast<size_t>(y_ceil) * static_cast<size_t>(input_soa.width)) + static_cast<size_t>(x_ceil);
            InterpolationParams const params_r{.c00 = input_soa.R[idx_yfloor_xfloor],.c10 = input_soa.R[idx_yfloor_xceil],.c01 = input_soa.R[idx_yceil_xfloor],.c11 = input_soa.R[idx_yceil_xceil],.x_frac = x_frac,.y_frac = y_frac};
            InterpolationParams const params_g{.c00 = input_soa.G[idx_yfloor_xfloor],.c10 = input_soa.G[idx_yfloor_xceil],.c01 = input_soa.G[idx_yceil_xfloor],.c11 = input_soa.G[idx_yceil_xceil],.x_frac = x_frac,.y_frac = y_frac};
            InterpolationParams const params_b{.c00 = input_soa.B[idx_yfloor_xfloor],.c10 = input_soa.B[idx_yfloor_xceil],.c01 = input_soa.B[idx_yceil_xfloor],.c11 = input_soa.B[idx_yceil_xceil],.x_frac = x_frac,.y_frac = y_frac};
            int const interpolated_r = interpolate(params_r);
            int const interpolated_g = interpolate(params_g);
            int const interpolated_b = interpolate(params_b);
            resized_soa.R.emplace_back(interpolated_r);
            resized_soa.G.emplace_back(interpolated_g);
            resized_soa.B.emplace_back(interpolated_b);
        }
    }
    resized_soa.width = new_width;resized_soa.height = new_height;
    return resized_soa;
}