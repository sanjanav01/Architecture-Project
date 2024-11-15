#include "imageaos.hpp"
#include "iostream"
#include "helpers/helpers.hpp" // Include the shared helper file
#include <map>
#include <vector>
#include "common/image_types.hpp"
#include <algorithm>

constexpr static int PixelResizeThreshold = 256;
constexpr static int BytesPerPixel3 = 3;
constexpr static int BytesPerPixel6 = 6;

ImageAOS::ImageAOS(int width, int height)
    : pixels(static_cast<size_t>(width * height)), width(width), height(height) {}

void ImageAOS::cutfreq(int frequency_threshold) {
    std::cout << "put code" << frequency_threshold << '\n';
    /* Extract Red, Green, and Blue channels from pixels
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
        pixels[i].r = channels.R[i];
        pixels[i].g = channels.G[i];
        pixels[i].b = channels.B[i];
    }*/
}

Image ImageAOS::resize(const Image &image, const int new_width, const int new_height) {
    Image resized_image(new_width, new_height);
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
            Pixel pix{};
            pix.r = static_cast<uint8_t>(
                ((1.0F - y_frac) * ((1.0F - x_frac) * static_cast<float>(R00) + x_frac * static_cast<float>(R10))) +
                (y_frac * ((1.0F - x_frac) * static_cast<float>(R01) + x_frac * static_cast<float>(R11)))
            );
            pix.g = static_cast<uint8_t>(
                ((1.0F - y_frac) * ((1.0F - x_frac) * static_cast<float>(G00) + x_frac * static_cast<float>(G10))) +
                (y_frac * ((1.0F - x_frac) * static_cast<float>(G01) + x_frac * static_cast<float>(G11)))
            );
            pix.b = static_cast<uint8_t>(
                ((1.0F - y_frac) * ((1.0F - x_frac) * static_cast<float>(B00) + x_frac * static_cast<float>(B10))) +
                (y_frac * ((1.0F - x_frac) * static_cast<float>(B01) + x_frac * static_cast<float>(B11)))
            );
            resized_image.pixels[(static_cast<size_t>(y_new) * static_cast<size_t>(new_width)) + static_cast<size_t>(x_new)] = pix;
        }
    }
    return resized_image;
}

void ImageAOS::maxlevel(Image& image, int new_max_value) {
    if (new_max_value <= 0) {
        throw std::invalid_argument("New max value must be positive.");
    }
    // Pre-compute scaling factor
    double const scaling_factor = static_cast<double>(new_max_value) / image.max_color_value;
    // Scale each pixel's RGB values using floor instead of round
    for (auto& pixel : image.pixels) {
        pixel.r = static_cast<uint16_t>(std::floor(static_cast<double>(pixel.r) * scaling_factor));
        pixel.g = static_cast<uint16_t>(std::floor(static_cast<double>(pixel.g) * scaling_factor));
        pixel.b = static_cast<uint16_t>(std::floor(static_cast<double>(pixel.b) * scaling_factor));

        // Optional: clamp each channel's value if desired to ensure it is within the [0, new_max_value] range
        pixel.r = std::clamp(pixel.r, static_cast<uint16_t>(0), static_cast<uint16_t>(new_max_value));
        pixel.g = std::clamp(pixel.g, static_cast<uint16_t>(0), static_cast<uint16_t>(new_max_value));
        pixel.b = std::clamp(pixel.b, static_cast<uint16_t>(0), static_cast<uint16_t>(new_max_value));
    }

    // Update the max_color_value in the image
    image.max_color_value = new_max_value;

    // Adjust internal storage based on pixel size requirement
    const size_t total_pixels = static_cast<size_t>(image.width) * static_cast<size_t>(image.height);
    const size_t required_size = (new_max_value >= PixelResizeThreshold)
                                 ? total_pixels * BytesPerPixel6
                                 : total_pixels * BytesPerPixel3;

    // Resize the pixel vector if required
    if (image.pixels.size() * sizeof(Pixel) < required_size) {
        image.pixels.resize(required_size / sizeof(Pixel));
    }
}

