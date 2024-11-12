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

    // Scaling factors for width and height
    float const x_scale = static_cast<float>(image.width) / static_cast<float>(new_width);
    float const y_scale = static_cast<float>(image.height) / static_cast<float>(new_height);

    // Nearest-neighbor interpolation loop
    for (int hgt = 0; hgt < new_height; ++hgt) {
        for (int wdt = 0; wdt < new_width; ++wdt) {
            // Calculate the nearest source coordinates in the original image
            auto src_x = static_cast<size_t>(std::round(static_cast<float>(hgt) * x_scale));
            auto src_y = static_cast<size_t>(std::round(static_cast<float>(wdt) * y_scale));
            src_x = std::min(src_x, static_cast<size_t>(image.width - 1));
            src_y = std::min(src_y, static_cast<size_t>(image.height - 1));

            // Set the pixel in the resized image
            resized_image.pixels[(static_cast<size_t>(hgt) * static_cast<size_t>(new_width)) + static_cast<size_t>(wdt)] =
                image.pixels[(src_y * static_cast<size_t>(image.width)) + src_x];
        }
    }
    return resized_image;
}
