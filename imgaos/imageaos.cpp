#include "imageaos.hpp"
#include "helpers/helpers.hpp" // Include the shared helper file
#include "common/image_types.hpp"      // Include the shared Pixel struct
#include <map>
#include <vector>

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
  // Update the pixels with new color values
  for (size_t i = 0; i < pixels.size(); ++i) {
    pixels[i].r = static_cast<uint16_t>(channels.R[i]);
    pixels[i].g = static_cast<uint16_t>(channels.G[i]);
    pixels[i].b = static_cast<uint16_t>(channels.B[i]);
  }
}


