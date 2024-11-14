#ifndef IMAGEAOS_HPP
#define IMAGEAOS_HPP

#include <vector>
#include <cstdint> // For uint16_t, uint32_t
#include <string>  // For std::string
#include "common/image_types.hpp"

struct RGB {
  uint16_t red;
  uint16_t green;
  uint16_t blue;
};

struct ImageAOS {
  std::vector<RGB> pixels;
  int width;
  int height;
  uint16_t maxColorValue = 0;

  // Explicit constructor to initialize width, height, and reserve space for pixels
  explicit ImageAOS(int width = 0, int height = 0) : width(width), height(height) {
    pixels.resize(static_cast<std::vector<RGB>::size_type>(std::max(width * height, 0)));
  }
};

struct CompressedImageAOS {
  std::string magicNumber;
  int width;
  int height;
  uint16_t maxColorValue;
  int colorTableSize;
  std::vector<uint16_t> colorTable;
  std::vector<uint32_t> pixelIndices;
};

// Function declaration for compress_aos, which now returns CompressedImage
CompressedImage compress_aos(const Image& image);
Image decompress(const CompressedImage& compressedImage);

#endif // IMAGEAOS_HPP
