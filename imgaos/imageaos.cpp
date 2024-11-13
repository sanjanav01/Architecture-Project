#include "imageaos.hpp"
#include "helpers/helpers.hpp" // Include the shared helper file
#include <map>
#include <vector>
#include "common/image_types.hpp"
#include <cmath>
#include <stdexcept>
#include <algorithm> // for std::clamp
#include <iostream>

// constexpr static int MaxByteValue = 255;
 constexpr static int PixelResizeThreshold = 256;
 constexpr static int BytesPerPixel3 = 3;
 constexpr static int BytesPerPixel6 = 6;

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
        uint16_t const original_r = pixel.r;
        uint16_t const original_g = pixel.g;
        uint16_t const original_b = pixel.b;

        pixel.r = static_cast<uint16_t>(std::floor(static_cast<double>(pixel.r) * scaling_factor));
        pixel.g = static_cast<uint16_t>(std::floor(static_cast<double>(pixel.g) * scaling_factor));
        pixel.b = static_cast<uint16_t>(std::floor(static_cast<double>(pixel.b) * scaling_factor));
        // Optional: clamp each channel's value if desired to ensure it is within the [0, new_max_value] range
        pixel.r = std::clamp(pixel.r, static_cast<uint16_t>(0), static_cast<uint16_t>(new_max_value));
        pixel.g = std::clamp(pixel.g, static_cast<uint16_t>(0), static_cast<uint16_t>(new_max_value));
        pixel.b = std::clamp(pixel.b, static_cast<uint16_t>(0), static_cast<uint16_t>(new_max_value));

        // Debugging output to check the values
        if (pixel.r != original_r || pixel.g != original_g || pixel.b != original_b) {
            std::cout << "Original RGB(" << original_r << ", " << original_g << ", " << original_b << ") -> "
                      << "Scaled RGB(" << pixel.r << ", " << pixel.g << ", " << pixel.b << ")\n";
        }
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
