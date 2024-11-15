#ifndef IMAGESOA_HPP
#define IMAGESOA_HPP
#include <vector>
#include "common/image_types.hpp"

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
    int width;
    int height;
    int current_max_color_value;
    std::vector<int> R;
    std::vector<int> G;
    std::vector<int> B;

    ImageSOA(Width width, Height height, MaxColorValue max_color_value);

    ImageSOA(Width width, Height height);

    ImageSOA(int width, int height);

    void cutfreq(int frequency_threshold);

    void maxlevel(int new_max_value);

    static ImageSOA resize_soa( int new_width, const ImageSOA &input_soa, int new_height);

};

CompressedImage compress_soa(const Image& image);
Image decompress_soa(const CompressedImage& compressedImage);
#endif