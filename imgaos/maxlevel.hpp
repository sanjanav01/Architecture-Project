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

  ImageAOS(int width, int height);

  void cutfreq(int frequency_threshold);

  // Function to scale pixel values to a new max level
  void maxlevel(int new_max_level);

  [[nodiscard]] std::map<std::tuple<int, int, int>, int> calculateColorFrequencies() const;

  // Static declarations (remove duplicates)
  [[nodiscard]] static std::vector<std::tuple<int, int, int>>
      getInfrequentColors(const std::map<std::tuple<int, int, int>, int>& color_freq,
                          int frequency_threshold);

  static std::tuple<int, int, int>
      findClosestColor(const std::tuple<int, int, int>& color,
                       const std::map<std::tuple<int, int, int>, int>& color_freq,
                       int frequency_threshold);

  void replaceInfrequentColors(const std::map<std::tuple<int, int, int>, int>& color_freq,
    const std::vector<std::tuple<int, int, int>>& infrequent_colors, int frequency_threshold);

};

#endif // IMAGEAOS_HPP