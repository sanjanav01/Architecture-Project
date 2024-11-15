#ifndef IMAGEAOS_HPP
#define IMAGEAOS_HPP

#include <vector>
#include "common/image_types.hpp"
#include <cstdint> // For uint16_t, uint32_t
#include <string>  // For std::string

struct RGB {
  uint16_t red;
  uint16_t green;
  uint16_t blue;
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

class ImageAOS {
  public:
  std::vector<Pixel> pixels;
  int width;
  int height;

  ImageAOS(int width, int height);

  void cutfreq(int frequency_threshold);
  static void maxlevel(Image & image, int new_max_value);
  static Image resize(const Image& image, int new_width, int new_height);
};

// Function declaration for compress_aos, which now returns CompressedImage
CompressedImage compress_aos(const Image& image);
Image decompress(const CompressedImage& compressedImage);
#endif // IMAGEAOS_HPP

