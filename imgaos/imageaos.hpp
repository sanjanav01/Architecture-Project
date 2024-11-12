#ifndef IMAGEAOS_HPP
#define IMAGEAOS_HPP

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

};
ImageAOS resize_aos(const ImageAOS& image, int new_width, int new_height);

#endif // IMAGEAOS_HPP



