// imageaos.hpp

#ifndef IMAGEAOS_HPP
#define IMAGEAOS_HPP

#include <map>
#include <tuple>
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

  // Constructor to initialize the image dimensions
  ImageAOS(int w, int h);

  // Function to remove infrequent colors
  void cutfreq(int frequency_threshold);
  std::map<std::tuple<int, int, int>, int> calculateColorFrequencies() const;
  std::vector<std::tuple<int, int, int>>
      getInfrequentColors(std::map<std::tuple<int, int, int>, int> const & color_freq,
                          int frequency_threshold) const;
  void replaceInfrequentColors(std::map<std::tuple<int, int, int>, int> const & color_freq,
                               std::vector<std::tuple<int, int, int>> const & infrequent_colors,
                               int frequency_threshold);
  std::tuple<int, int, int>
      findClosestColor(std::tuple<int, int, int> const & color,
                       std::map<std::tuple<int, int, int>, int> const & color_freq,
                       int frequency_threshold) const;

private:
  // Helper function to find the closest replacement color
  std::tuple<int, int, int> findClosestColor(const std::tuple<int, int, int>& color,
                                             const std::map<std::tuple<int, int, int>, int>& color_freq,
                                             int frequency_threshold);
};

#endif // IMAGEAOS_HPP


