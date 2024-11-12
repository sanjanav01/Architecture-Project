#ifndef IMAGEAOS_HPP
#define IMAGEAOS_HPP

#include <vector>
#include "common/image_types.hpp"

class ImageAOS {
  public:
  std::vector<Pixel> pixels;
  int width;
  int height;

  ImageAOS(int width, int height);

  void cutfreq(int frequency_threshold);
  static void maxlevel(Image & image, int new_max_value);
};

#endif // IMAGEAOS_HPP

