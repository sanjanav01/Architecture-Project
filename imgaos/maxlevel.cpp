#include "imageaos.hpp"
#include <cmath>
#include <stdexcept>

constexpr int static MaxByteValue = 255;
constexpr int static PixelResizeValue = 256;
constexpr int static BytesPerPixel3 = 3;
constexpr int static BytesPerPixel6 = 6;

void ImageAOS::maxlevel(int const new_max_value) {
  if (new_max_value <= 0) {
    throw std::invalid_argument("New max value must be positive.");
  }

  for (auto& pixel : pixels) {
    pixel.R = static_cast<int>(std::round((static_cast<double>(pixel.R) * new_max_value) / MaxByteValue));
    pixel.G = static_cast<int>(std::round((static_cast<double>(pixel.G) * new_max_value) / MaxByteValue));
    pixel.B = static_cast<int>(std::round((static_cast<double>(pixel.B) * new_max_value) / MaxByteValue));

    // Ensure the values are clamped to the new max value if rounding exceeded limits
    pixel.R = std::clamp(pixel.R, 0, new_max_value);
    pixel.G = std::clamp(pixel.G, 0, new_max_value);
    pixel.B = std::clamp(pixel.B, 0, new_max_value);
  }

  // Adjust bytes per pixel representation
  if (new_max_value >= PixelResizeValue && static_cast<size_t>(width) * static_cast<size_t>(height) * BytesPerPixel6 > pixels.size()) {
    pixels.resize(static_cast<size_t>(width) * static_cast<size_t>(height) * BytesPerPixel6);
  } else if (new_max_value < PixelResizeValue && static_cast<size_t>(width) * static_cast<size_t>(height) * BytesPerPixel3 > pixels.size()) {
    pixels.resize(static_cast<size_t>(width) * static_cast<size_t>(height) * BytesPerPixel3);
  }
}
