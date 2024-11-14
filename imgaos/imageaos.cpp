#include "imageaos.hpp"
#include "helpers/helpers.hpp" // Include the shared helper file
#include <map>
#include <vector>
#include "common/image_types.hpp"
#include <algorithm>

constexpr static float MAGICF = 255.0F;
// Constructor with width and height parameters
ImageAOS::ImageAOS(int width, int height)
    : pixels(static_cast<size_t>(width * height)), width(width), height(height) {}

// Main cutfreq function, which uses shared helper functions for color analysis
void ImageAOS::cutfreq(int frequency_threshold) {
    // Extract Red, Green, and Blue channels from pixels
    ColorChannels channels;
    for (const auto& pixel : pixels) {
        channels.R.push_back(pixel.r);
        channels.G.push_back(pixel.g);
        channels.B.push_back(pixel.b);
    }

    auto color_freq = calculateColorFrequencies(channels);
    auto infrequent_colors = getInfrequentColors(color_freq, frequency_threshold);
    replaceInfrequentColors(channels, color_freq, frequency_threshold);

    // Update the pixels with new color values
    for (size_t i = 0; i < pixels.size(); ++i) {
        pixels[i].r = static_cast<uint16_t>(channels.R[i]);
        pixels[i].g = static_cast<uint16_t>(channels.G[i]);
        pixels[i].b = static_cast<uint16_t>(channels.B[i]);
    }
}

Image ImageAOS::resize_aos(const Image& image, const int new_width, const int new_height) {
    Image resized_image(new_width, new_height);
    const float x_scale = static_cast<float>(image.width - 1) / static_cast<float>(new_width - 1);
    const float y_scale = static_cast<float>(image.height - 1) / static_cast<float>(new_height - 1);

    //const float xcoord = (image.width * new_width) / image.width;
    //const float ycoord = ( * image.height) / new_height;
    for (int y_new = 0; y_new < new_height; ++y_new) {
        //const int xlow = static_cast<int>(std::floor(xcoord));
        //const int xceil = static_cast<int>(std::ceil(xcoord));
        //const int ylow = static_cast<int>(std::floor(ycoord));
        //const int yceil = static_cast<int>(std::ceil(ycoord));

        float const yscale = static_cast<float>(y_new) * y_scale;
        int const yfloor = static_cast<int>(yscale);
        int const yfloor1 = std::min(yfloor + 1, image.height - 1);
        float const y_frac = yscale - static_cast<float>(yfloor);
        for (int x_new = 0; x_new < new_width; ++x_new) {
            float const xscale = static_cast<float>(x_new) * x_scale;
            int const xfloor = static_cast<int>(xscale);
            int const xfloor1 = std::min(xfloor + 1, image.width - 1);
            float const x_frac = xscale - static_cast<float>(xfloor);
            const auto& [R00, G00, B00] = image.pixels[(static_cast<size_t>(yfloor) * static_cast<size_t>(image.width)) + static_cast<size_t>(xfloor)];
            const auto& [R10, G10, B10] = image.pixels[(static_cast<size_t>(yfloor) * static_cast<size_t>(image.width)) + static_cast<size_t>(xfloor1)];
            const auto& [R01, G01, B01] = image.pixels[(static_cast<size_t>(yfloor1) * static_cast<size_t>(image.width)) + static_cast<size_t>(xfloor)];
            const auto& [R11, G11, B11] = image.pixels[(static_cast<size_t>(yfloor1) * static_cast<size_t>(image.width)) + static_cast<size_t>(xfloor1)];
            auto interpolateAndClamp = [](float v00, float v10, float v01, float v11, float x_frac, float y_frac) {
                float const interpolated = ((1.0F - y_frac) * ((1.0F - x_frac) * v00 + x_frac * v10)) +(y_frac * ((1.0F - x_frac) * v01 + x_frac * v11));
                return static_cast<uint16_t>(std::clamp(std::round(interpolated), 0.0F, MAGICF));
                //return static_cast<uint16_t>(std::clamp(std::floor(interpolated), 0.0F, MAGICF));
            };
            Pixel pix{};
            pix.r = interpolateAndClamp(static_cast<float>(R00), static_cast<float>(R10),static_cast<float>(R01), static_cast<float>(R11),x_frac, y_frac);
            pix.g = interpolateAndClamp(static_cast<float>(G00), static_cast<float>(G10),static_cast<float>(G01), static_cast<float>(G11),x_frac, y_frac);
            pix.b = interpolateAndClamp(static_cast<float>(B00), static_cast<float>(B10),static_cast<float>(B01), static_cast<float>(B11),x_frac, y_frac);
            resized_image.pixels[(static_cast<size_t>(y_new) * static_cast<size_t>(new_width)) + static_cast<size_t>(x_new)] = pix;
        }
    }
    return resized_image;
}
/*Image ImageAOS::resize_aos(const Image& image, const int new_width, const int new_height) {
    Image resized_image(new_width, new_height);
    const float x_scale = static_cast<float>(image.width) / static_cast<float>(new_width);
    const float y_scale = static_cast<float>(image.height) / static_cast<float>(new_height);
    for (int y_new = 0; y_new < new_height; ++y_new) {
        float const yscale = static_cast<float>(y_new) * y_scale;
        int const yfloor = static_cast<int>(std::floor(yscale));
        int const yhigh = std::min(yfloor + 1, image.height - 1);
        float const y_frac = yscale - static_cast<float>(yfloor);
        for (int x_new = 0; x_new < new_width; ++x_new) {
            float const xscale = static_cast<float>(x_new) * x_scale;
            int const xfloor = static_cast<int>(std::floor(xscale));
            int const xhigh = std::min(xfloor + 1, image.width - 1);
            float const x_frac = xscale - static_cast<float>(xfloor);
            const auto& [R00, G00, B00] = image.pixels[(static_cast<size_t>(yfloor) * static_cast<size_t>(image.width)) + static_cast<size_t>(xfloor)];
            const auto& [R10, G10, B10] = image.pixels[(static_cast<size_t>(yfloor) * static_cast<size_t>(image.width)) + static_cast<size_t>(xhigh)];
            const auto& [R01, G01, B01] = image.pixels[(static_cast<size_t>(yhigh) * static_cast<size_t>(image.width)) + static_cast<size_t>(xfloor)];
            const auto& [R11, G11, B11] = image.pixels[(static_cast<size_t>(yhigh) * static_cast<size_t>(image.width)) + static_cast<size_t>(xhigh)];
            float const R_x1 = ((1 - x_frac) * static_cast<float>(R00)) + (x_frac * static_cast<float>(R10));
            float const R_x2 = ((1 - x_frac) * static_cast<float>(R01)) + (x_frac * static_cast<float>(R11));
            float const G_x1 = ((1 - x_frac) * static_cast<float>(G00)) + (x_frac * static_cast<float>(G10));
            float const G_x2 = ((1 - x_frac) * static_cast<float>(G01)) + (x_frac * static_cast<float>(G11));
            float const B_x1 = ((1 - x_frac) * static_cast<float>(B00)) + (x_frac * static_cast<float>(B10));
            float const B_x2 = ((1 - x_frac) * static_cast<float>(B01)) + (x_frac * static_cast<float>(B11));
            uint16_t const R_new = static_cast<uint16_t>(std::clamp(std::round(((1 - y_frac) * R_x1) + (y_frac * R_x2)), 0.0F, 255.0F));
            uint16_t const G_new = static_cast<uint16_t>(std::clamp(std::round(((1 - y_frac) * G_x1) + (y_frac * G_x2)), 0.0F, 255.0F));
            uint16_t const B_new = static_cast<uint16_t>(std::clamp(std::round(((1 - y_frac) * B_x1) + (y_frac * B_x2)), 0.0F, 255.0F));
            resized_image.pixels[(static_cast<size_t>(y_new) * static_cast<size_t>(new_width)) + static_cast<size_t>(x_new)] = {.r=R_new, .g=G_new, .b=B_new};
        }
    }
    return resized_image;
}
*/
