#include "imagesoa.hpp"
#include "helpers/helpers.hpp" // Include the shared helper file
#include <map>
#include <vector>

// Constructor with width and height parameters
ImageSOA::ImageSOA(int width, int height)
    : R(static_cast<size_t>(width * height)), G(static_cast<size_t>(width * height)),
      B(static_cast<size_t>(width * height)), width(width), height(height) {}

// Main cutfreq function, which uses shared helper functions for color analysis
void ImageSOA::cutfreq(int frequency_threshold) {
  // Create a ColorChannels instance to group R, G, and B channels
  ColorChannels channels = {.R=R, .G=G, .B=B};

  auto color_freq = calculateColorFrequencies(channels);
  auto infrequent_colors = getInfrequentColors(color_freq, frequency_threshold);
  replaceInfrequentColors(channels, color_freq, frequency_threshold);

  // Update the original R, G, B channels with the modified values
  R = channels.R;
  G = channels.G;
  B = channels.B;
}

