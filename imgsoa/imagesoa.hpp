#ifndef IMGSOA_HPP
#define IMGSOA_HPP

#include <vector>
#include <cstdint> // For uint16_t, uint32_t
#include <string>  // For std::string

struct ImageSOA {
  std::vector<uint16_t> red;
  std::vector<uint16_t> green;
  std::vector<uint16_t> blue;
  int width;
  int height;
  uint16_t maxColorValue;
};

struct CompressedImageSOA {
  std::string magicNumber;
  int width;
  int height;
  uint16_t maxColorValue;
  int colorTableSize;
  std::vector<uint16_t> colorTable;
  std::vector<uint32_t> pixelIndices;
};

CompressedImageSOA compress_soa(const ImageSOA& image);

#endif // IMGSOA_HPP
