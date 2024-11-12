#ifndef IMAGEAOS_HPP
#define IMAGEAOS_HPP

#include <vector>

struct Pixel {
    mutable int R;
    mutable int G;
    mutable int B;
};

class ImageAOS {
public:
    std::vector<Pixel> pixels;
    int width;
    int height;

    ImageAOS(int width, int height);

    ImageAOS(int width, int height, uint16_t uint16);

    void cutfreq(int frequency_threshold);

};
ImageAOS resize_aos(const ImageAOS& image, int new_width, int new_height);

#endif // IMAGEAOS_HPP



