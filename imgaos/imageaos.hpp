#ifndef IMAGEAOS_HPP
#define IMAGEAOS_HPP
#include "common/image_types.hpp"


#include <vector>

class ImageAOS {
  public:
  std::vector<Pixel> pixels;
  int width;
  int height;

  ImageAOS(int width, int height);

  void cutfreq(int frequency_threshold);

};

#endif // IMAGEAOS_HPP

