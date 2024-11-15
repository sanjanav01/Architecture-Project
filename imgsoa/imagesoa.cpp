#include "imagesoa.hpp"
#include "helpers/helpers.hpp" // Include the shared helper file
#include <map>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <algorithm> // for std::clamp
#include <iostream> // for std::clamp

// Constants for byte sizes based on max color value
constexpr static int MAX_VAL= 255;
constexpr static int PixelThreshold= 256;



ImageSOA::ImageSOA(Width width, Height height, MaxColorValue max_color_value)
    : width(width.value),  // Initialize width
      height(height.value), // Initialize height
      current_max_color_value(max_color_value.value), // Initialize max color value
      R(static_cast<size_t>(width.value * height.value)),      // Initialize R
      G(static_cast<size_t>(width.value * height.value)),      // Initialize G
      B(static_cast<size_t>(width.value * height.value))       // Initialize B
{}

ImageSOA::ImageSOA(Width width, Height height)
    : width(width.value),  // Initialize width
      height(height.value), // Initialize height
      current_max_color_value(MAX_VAL), // Default max color value
      R(static_cast<size_t>(width.value * height.value)),      // Initialize R
      G(static_cast<size_t>(width.value * height.value)),      // Initialize G
      B(static_cast<size_t>(width.value * height.value))       // Initialize B
{}


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

void ImageSOA::maxlevel(int new_max_value) {
  if (new_max_value <= 0) {
    throw std::invalid_argument("New max value must be positive.");
  }
  // Pre-compute scaling factor
  double const scaling_factor = static_cast<double>(new_max_value) / current_max_color_value;
  // Scale each RGB component independently
  for (size_t i = 0; i < R.size(); ++i) {
    // Apply scaling with rounding
    R[i] = static_cast<int>(std::floor(static_cast<double>(R[i]) * scaling_factor));
    G[i] = static_cast<int>(std::floor(static_cast<double>(G[i]) * scaling_factor));
    B[i] = static_cast<int>(std::floor(static_cast<double>(B[i]) * scaling_factor));
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
    // 3 bytes per pixel
    if (R.capacity() < total_pixels || G.capacity() < total_pixels || B.capacity() < total_pixels) {
      R.resize(total_pixels);
      G.resize(total_pixels);
      B.resize(total_pixels);
    }
    std::cout << "Switching to 3-byte representation (max color value < 256).\n";
  } else {
    // 6 bytes per pixel
    if (R.capacity() < 2 * total_pixels || G.capacity() < 2 * total_pixels || B.capacity() < 2 * total_pixels) {
      R.resize(2 * total_pixels);
      G.resize(2 * total_pixels);
      B.resize(2 * total_pixels);
    }
    std::cout << "Switching to 6-byte representation (max color value >= 256).\n";
  }
}

