#ifndef IMAGESOA_HPP
#define IMAGESOA_HPP

#include <vector>

// Wrapper types for safer parameter passing
struct Width {
  int value;
};

struct Height {
  int value;
};

struct MaxColorValue {
  int value;
};

class ImageSOA {
  public:
  int width;                    // First member
  int height;                   // Second member
  int current_max_color_value;  // Third member
  std::vector<int> R;           // Fourth member
  std::vector<int> G;           // Fifth member
  std::vector<int> B;           // Sixth member

  // Constructor to initialize the image dimensions
  ImageSOA(Width width, Height height, MaxColorValue max_color_value);

  // Overloaded constructor for cases without max_color_value
  ImageSOA(Width width, Height height);

  // Function to remove infrequent colors
  void cutfreq(int frequency_threshold);

  // Function to apply maxlevel scaling
  void maxlevel(int new_max_value);
};

#endif // IMAGESOA_HPP
