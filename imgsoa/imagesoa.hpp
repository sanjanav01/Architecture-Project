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

    [[nodiscard]] ImageSOA resize_soa(int new_width, int new_height) const;
};

#endif // IMAGESOA_HPP
