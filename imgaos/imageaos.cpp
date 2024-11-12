#include "imageaos.hpp"
#include "helpers/helpers.hpp" // Include the shared helper file
#include <map>
#include <vector>

// Constructor with width and height parameters
ImageAOS::ImageAOS(int width, int height)
    : pixels(static_cast<size_t>(width * height)), width(width), height(height) {}

// Main cutfreq function, which uses shared helper functions for color analysis
void ImageAOS::cutfreq(int frequency_threshold) {
    // Extract Red, Green, and Blue channels from pixels
    ColorChannels channels;
    for (const auto& pixel : pixels) {
        channels.R.push_back(pixel.R);
        channels.G.push_back(pixel.G);
        channels.B.push_back(pixel.B);
    }

    auto color_freq = calculateColorFrequencies(channels);
    auto infrequent_colors = getInfrequentColors(color_freq, frequency_threshold);
    replaceInfrequentColors(channels, color_freq, frequency_threshold);

    // Update the pixels with new color values
    for (size_t i = 0; i < pixels.size(); ++i) {
        pixels[i].R = channels.R[i];
        pixels[i].G = channels.G[i];
        pixels[i].B = channels.B[i];
    }
}

ImageAOS resize_aos(const ImageAOS& image, const int new_width, const int new_height) {
    ImageAOS resized_image(new_width, new_height);
    float const x_scale = static_cast<float>(image.width - 1) / static_cast<float>(new_width - 1);
    float const y_scale = static_cast<float>(image.height - 1) / static_cast<float>(new_height - 1);
    for (int y_new = 0; y_new < new_height; ++y_new) {
        float const yscale = static_cast<float>(y_new) * y_scale;
        int const yfloor = static_cast<int>(yscale);
        int const yfloor1 = std::min(yfloor + 1, image.height - 1);
        float const y_frac = yscale - static_cast<float>(yfloor);
        for (int x_new = 0; x_new < new_width; ++x_new) {
            float const xscale = static_cast<float>(x_new) * x_scale;
            int const xfloor = static_cast<int>(xscale);
            int const xfloor1 = std::min(xfloor + 1, image.width - 1);
            float const x_frac = xscale - static_cast<float>(xfloor);
            const auto&[R00, G00, B00] = image.pixels[(static_cast<size_t>(yfloor) * static_cast<size_t>(image.width)) + static_cast<size_t>(xfloor)];
            const auto&[R10, G10, B10] = image.pixels[(static_cast<size_t>(yfloor) * static_cast<size_t>(image.width)) + static_cast<size_t>(xfloor1)];
            const auto&[R01, G01, B01] = image.pixels[(static_cast<size_t>(yfloor1) * static_cast<size_t>(image.width)) + static_cast<size_t>(xfloor)];
            const auto&[R11, G11, B11] = image.pixels[(static_cast<size_t>(yfloor1) * static_cast<size_t>(image.width)) + static_cast<size_t>(xfloor1)];
            constexpr Pixel pix{};
            pix.R = static_cast<uint8_t>(
                ((1.0F - y_frac) * ((1.0F - x_frac) * static_cast<float>(R00) + x_frac * static_cast<float>(R10))) +
                (y_frac * ((1.0F - x_frac) * static_cast<float>(R01) + x_frac * static_cast<float>(R11)))
            );
            pix.G = static_cast<uint8_t>(
                ((1.0F - y_frac) * ((1.0F - x_frac) * static_cast<float>(G00) + x_frac * static_cast<float>(G10))) +
                (y_frac * ((1.0F - x_frac) * static_cast<float>(G01) + x_frac * static_cast<float>(G11)))
            );
            pix.B = static_cast<uint8_t>(
                ((1.0F - y_frac) * ((1.0F - x_frac) * static_cast<float>(B00) + x_frac * static_cast<float>(B10))) +
                (y_frac * ((1.0F - x_frac) * static_cast<float>(B01) + x_frac * static_cast<float>(B11)))
            );
            resized_image.pixels[(static_cast<size_t>(y_new) * static_cast<size_t>(new_width)) + static_cast<size_t>(x_new)] = pix;
        }
    }
    return resized_image;
}