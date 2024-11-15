// imagesoa.hpp
// imagesoa.hpp

#ifndef IMAGESOA_HPP
#define IMAGESOA_HPP

#include <vector>
#include "common/image_types.hpp"

class ImageSOA {
public:
    std::vector<int> R;
    std::vector<int> G;
    std::vector<int> B;
    int current_max_color_value{};
    int width;
    int height;

    // Constructor to initialize the image dimensions
    ImageSOA(int width, int height, int max_color_value);

    ImageSOA(int width, int height);

    // Function to remove infrequent colors
    void cutfreq(int frequency_threshold);
    void maxlevel(int new_max_value);
    static ImageSOA resize_soa(const ImageSOA& input_soa, int new_width, int new_height);

};

#endif // IMAGESOA_HPP