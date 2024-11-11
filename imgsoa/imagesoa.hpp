// imagesoa.hpp

#ifndef IMAGESOA_HPP
#define IMAGESOA_HPP

#include <map>
#include <tuple>
#include <vector>

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

  // Helper functions
  std::map<std::tuple<int, int, int>, int> calculateColorFrequencies() const;
  std::vector<std::tuple<int, int, int>> getInfrequentColors(
      const std::map<std::tuple<int, int, int>, int>& color_freq,
      int frequency_threshold) const;
  void replaceInfrequentColors(
      const std::map<std::tuple<int, int, int>, int>& color_freq,
      const std::vector<std::tuple<int, int, int>>& infrequent_colors,
      int frequency_threshold);

  private:
  std::tuple<int, int, int> findClosestColor(
      const std::tuple<int, int, int>& color,
      const std::map<std::tuple<int, int, int>, int>& color_freq,
      int frequency_threshold) const;
};

#endif // IMAGESOA_HPP
