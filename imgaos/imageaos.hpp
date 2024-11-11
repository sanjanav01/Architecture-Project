// imageaos.hpp

#ifndef IMAGEAOS_HPP
#define IMAGEAOS_HPP

#include <map>
#include <tuple>
#include <vector>
#include <helpers/helpers.hpp>

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
// Declare the resize function outside the class
ImageAOS resize_aos(const ImageAOS& image, int new_width, int new_height);

#endif // IMAGEAOS_HPP

