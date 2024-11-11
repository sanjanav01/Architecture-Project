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

  ImageAOS(int w, int h);

  void cutfreq(int frequency_threshold);
  std::map<std::tuple<int, int, int>, int> calculateColorFrequencies() const;
  std::vector<std::tuple<int, int, int>> getInfrequentColors(
      const std::map<std::tuple<int, int, int>, int>& color_freq, int frequency_threshold) const;
  void replaceInfrequentColors(
      const std::map<std::tuple<int, int, int>, int>& color_freq,
      const std::vector<std::tuple<int, int, int>>& infrequent_colors, int frequency_threshold);

  private:
  std::tuple<int, int, int> findClosestColor(
      const std::tuple<int, int, int>& color,
      const std::map<std::tuple<int, int, int>, int>& color_freq, int frequency_threshold) const;
};

#endif // IMAGEAOS_HPP

