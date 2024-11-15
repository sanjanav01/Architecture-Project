#ifndef IMAGESOA_HPP
#define IMAGESOA_HPP

#include <vector>
#include "common/image_types.hpp" // For Image, Pixel, etc.

// Wrapper types for safer parameter passing
struct Width {
  int value;
};

struct Height {
  int value;
};

struct MaxColorValue {
  int value;
};

class ImageSOA {
  public:
  int width;                    // First member
  int height;                   // Second member
  int current_max_color_value;  // Third member
  std::vector<int> R;           // Fourth member
  std::vector<int> G;           // Fifth member
  std::vector<int> B;           // Sixth member


  ImageSOA(Width width, Height height, MaxColorValue max_color_value);

  ImageSOA(Width width, Height height);

  ImageSOA(int width, int height);

  void cutfreq(int frequency_threshold);

  void maxlevel(int new_max_value);

  static ImageSOA resize_soa( int new_width, const ImageSOA &input_soa, int new_height);

};

// Function declarations
CompressedImage compress_soa(const Image& image);
Image decompress_soa(const CompressedImage& compressedImage);
#endif // IMAGESOA_HPP
