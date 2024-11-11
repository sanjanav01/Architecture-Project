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
  ImageSOA(int width, int height);

  // Function to remove infrequent colors
  void cutfreq(int frequency_threshold);

  // Helper functions
  [[nodiscard]] std::map<std::tuple<int, int, int>, int> calculateColorFrequencies() const;
  static std::vector<std::tuple<int, int, int>> getInfrequentColors(
      const std::map<std::tuple<int, int, int>, int>& color_freq,
      int frequency_threshold) ;
  void replaceInfrequentColors(
      const std::map<std::tuple<int, int, int>, int>& color_freq,
      const std::vector<std::tuple<int, int, int>>& infrequent_colors,
      int frequency_threshold);
  [[nodiscard]] ImageSOA resize_soa(int new_width, int new_height) const;

  private:
  static std::tuple<int, int, int> findClosestColor(
      const std::tuple<int, int, int>& color,
      const std::map<std::tuple<int, int, int>, int>& color_freq,
      int frequency_threshold) ;
};

#endif // IMAGESOA_HPP