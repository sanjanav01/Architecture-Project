// imageaos.hpp

#ifndef IMAGEAOS_HPP
#define IMAGEAOS_HPP

#include <vector>
#include <tuple>

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

  // Constructor to initialize the image dimensions
  ImageAOS(int w, int h);

  // Function to remove infrequent colors
  void cutfreq(int frequency_threshold);

  private:
  // Helper function to find the closest replacement color
  std::tuple<int, int, int> findClosestColor(const std::tuple<int, int, int>& color,
                                             const std::map<std::tuple<int, int, int>, int>& color_freq,
                                             int frequency_threshold);
};

#endif // IMAGEAOS_HPP


