#ifndef IMGAOS_HPP
#define IMGAOS_HPP

#include <vector>
#include <cstdint> // For uint16_t, uint32_t
#include <string>  // For std::string

struct RGB {
  uint16_t red;
  uint16_t green;
  uint16_t blue;
};

struct ImageAOS {
  std::vector<RGB> pixels;
  int width;
  int height;
  uint16_t maxColorValue;

  // Explicit constructor to initialize width, height, and reserve space for pixels
  explicit ImageAOS(int w = 0, int h = 0) : width(w), height(h), maxColorValue(0) {
    pixels.resize(static_cast<std::vector<RGB>::size_type>(std::max(w * h, 0)));
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

CompressedImageAOS compress_aos(const ImageAOS& image);

#endif // IMGAOS_HPP
