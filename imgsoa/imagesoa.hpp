// imagesoa.hpp

#ifndef IMAGESOA_HPP
#define IMAGESOA_HPP

#include <vector>
#include <tuple>

class ImageSOA {
  public:
  std::vector<int> R;
  std::vector<int> G;
  std::vector<int> B;
  int width;
  int height;

  // Constructor to initialize the image dimensions
  ImageSOA(int w, int h);

  // Function to remove infrequent colors
  void cutfreq(int frequency_threshold);

  private:
  // Helper function to find the closest replacement color
  std::tuple<int, int, int> findClosestColor(const std::tuple<int, int, int>& color,
                                             const std::map<std::tuple<int, int, int>, int>& color_freq,
                                             int frequency_threshold);
};

#endif // IMAGESOA_HPP
