#ifndef IMGSOA_HPP
#define IMGSOA_HPP

#include <vector>
#include <cstdint> // For uint16_t, uint32_t
#include "common/image_types.hpp" // For Image, Pixel, etc.

struct ImageSOA {
  std::vector<uint16_t> red;
  std::vector<uint16_t> green;
  std::vector<uint16_t> blue;
  int width;
  int height;
  uint16_t maxColorValue;
};

// CompressedImage should already be defined in image_types.hpp or a related file

// Function declarations
CompressedImage compress_soa(const Image& image);
Image decompress_soa(const CompressedImage& compressedImage);

#endif // IMGSOA_HPP
