#ifndef IMAGEAOS_HPP
#define IMAGEAOS_HPP

#include <vector>

struct Pixel {
  int R;
  int G;
  int B;
};

class ImageAOS {
  public:
  std::vector<Pixel> pixels;
  int width;
  int height;

  ImageAOS(int width, int height);

  void cutfreq(int frequency_threshold);

};

#endif // IMAGEAOS_HPP

