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
};

struct CompressedImageAOS {
  std::string magicNumber; // Now std::string should be recognized
  int width;
  int height;
  uint16_t maxColorValue;
  int colorTableSize;
  std::vector<uint16_t> colorTable;
  std::vector<uint32_t> pixelIndices;
};

CompressedImageAOS compress_aos(const ImageAOS& image);

#endif // IMGAOS_HPP
