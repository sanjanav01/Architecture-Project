#ifndef IMAGEAOS_HPP
#define IMAGEAOS_HPP

#include <vector>
#include "common/image_types.hpp"

class ImageAOS {
public:
    std::vector<Pixel> pixels;
    int width;
    int height;

    ImageAOS(int width, int height);

    static void cutfreq(int frequency_threshold);
    static Image resize(const Image& image, int new_width, int new_height);
    static void maxlevel(Image &image, int new_max_value);
    CompressedImage compress_aos(const Image& image);
};
#endif // IMAGEAOS_HPP
