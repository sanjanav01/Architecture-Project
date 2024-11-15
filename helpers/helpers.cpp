#include "helpers.hpp"
#include "common/image_types.hpp"
#include <cmath>
#include <limits>
#include <cstdlib>
#include <string>
#include <iostream>
#include <tuple>
#include <imgsoa/imagesoa.hpp>
constexpr static int FIVE = 5;

bool compareImageAndSOA(const Image& image, const ImageSOA& soa_image) {
    if (image.width != soa_image.width || image.height != soa_image.height) {
        std::cout << "diff dimensions. Image: " << image.width << "x" << image.height
                  << " vs SOA Image: " << soa_image.width << "x" << soa_image.height << "\n";
        return false;
    }
    bool identical = true;
    for (int hgt = 0; hgt < image.height; ++hgt) {
        for (int wdt = 0; wdt < image.width; ++wdt) {
            const size_t index = (static_cast<size_t>(hgt) * static_cast<size_t>(image.width)) + static_cast<size_t>(wdt);
            const Pixel& img_pixel = image.pixels[index];
            const int soa_r = soa_image.R[index];
            const int soa_g = soa_image.G[index];
            const int soa_b = soa_image.B[index];
            if (std::abs(img_pixel.r - soa_r) > FIVE || std::abs(img_pixel.g - soa_g) > FIVE || std::abs(img_pixel.b - soa_b) > FIVE) {
                identical = false;
                std::cout << "diff at (" << wdt << ", " << hgt << "): "
                          << "image RGB(" << img_pixel.r << ", " << img_pixel.g << ", " << img_pixel.b << ") vs "
                          << "soa RGB(" << soa_r << ", " << soa_g << ", " << soa_b << ")\n";
            }
        }
    }
    if (identical) {
        std::cout << "equal soa and image\n";
    }
    return identical;
}

bool compareImagesByPixel(const Image& image1, const Image& image2) {
    if (image1.width != image2.width || image1.height != image2.height) {
        std::cout << image1.width << " x " << image1.height << '\n';
        std::cout << image2.width << " x " << image2.height << '\n';
        std::cout << "diff dimensions" << '\n';
        return false;
    }
    int total = 0;
    int diff = 0;
    bool equivalent = true;
    for (int hgt = 0; hgt < image1.height; ++hgt) {
        for (int wdt = 0; wdt < image1.width; ++wdt) {
            total += 1;
            const size_t index = (static_cast<size_t>(hgt) * static_cast<size_t>(image1.width)) + static_cast<size_t>(wdt);
            const auto& pix1 = image1.pixels[index];
            const auto& pix2 = image2.pixels[index];
            if (std::abs(pix1.r - pix2.r) > FIVE || std::abs(pix1.g - pix2.g) > FIVE || std::abs(pix1.b - pix2.b) > FIVE) {
                equivalent = false;
                diff += 1;
                std::cout << "Diff at (" << wdt << ", " << hgt << "): "
                          << "Image1 RGB(" << pix1.r << ", " << pix1.g << ", " << pix1.b << ") vs "
                          << "Image2 RGB(" << pix2.r << ", " << pix2.g << ", " << pix2.b << ")\n";
            }
        }
    }
    //std::cout << "Total Pixels: " << total << "Diff Pixels: " << diff << "\n";
    if (equivalent) {
        std::cout << "Equal <=5" << '\n';
    }
    return equivalent;
}


bool compareImages(const std::string& file1, const std::string& file2) {
    std::string const command = "cmp -s " + file1 + " " + file2;
    int const result = std::system(command.c_str());
    return result == 0;
}

std::map<std::tuple<int, int, int>, int> calculateColorFrequencies(
    const ColorChannels& channels) {
    std::map<std::tuple<int, int, int>, int> color_freq;
    for (size_t i = 0; i < channels.R.size(); ++i) {
        auto color = std::make_tuple(channels.R[i], channels.G[i], channels.B[i]);
        color_freq[color]++;
    }
    return color_freq;
}

std::vector<std::tuple<int, int, int>> getInfrequentColors(
    const std::map<std::tuple<int, int, int>, int>& color_freq,
    int frequency_threshold) {
    std::vector<std::tuple<int, int, int>> infrequent_colors;
    for (const auto& [color, freq] : color_freq) {
        if (freq < frequency_threshold) {
            infrequent_colors.push_back(color);
        }
    }
    return infrequent_colors;
}

void replaceInfrequentColors(
    ColorChannels& channels,
    const std::map<std::tuple<int, int, int>, int>& color_freq,
    int frequency_threshold) {

    for (size_t i = 0; i < channels.R.size(); ++i) {
        auto color = std::make_tuple(channels.R[i], channels.G[i], channels.B[i]);
        if (color_freq.at(color) < frequency_threshold) {
            auto [newR, newG, newB] = findClosestColor(color, color_freq, frequency_threshold);
            channels.R[i] = newR;
            channels.G[i] = newG;
            channels.B[i] = newB;
        }
    }
}

std::tuple<int, int, int> findClosestColor(
    const std::tuple<int, int, int>& color,
    const std::map<std::tuple<int, int, int>, int>& color_freq,
    int frequency_threshold) {

    double min_distance = std::numeric_limits<double>::max();
    std::tuple<int, int, int> closest_color;

    for (const auto& [frequent_color, freq] : color_freq) {
        if (freq >= frequency_threshold) {
            const double distance = std::sqrt(
                std::pow(std::get<0>(color) - std::get<0>(frequent_color), 2) +
                std::pow(std::get<1>(color) - std::get<1>(frequent_color), 2) +
                std::pow(std::get<2>(color) - std::get<2>(frequent_color), 2)
            );

            if (distance < min_distance) {
                min_distance = distance;
                closest_color = frequent_color;
            }
        }
    }

    return closest_color;
}

