// imagesoa.hpp

#ifndef IMAGESOA_HPP
#define IMAGESOA_HPP

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

};

#endif // IMAGESOA_HPP
