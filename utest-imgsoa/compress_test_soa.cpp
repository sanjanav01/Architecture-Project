#include "imagesoa.hpp"
#include "common/image_types.hpp"  // Includes definitions for Image and Pixel
#include <iostream>
#include <cassert>

namespace {
    // Custom function to compare two Pixel structs
    bool pixelsEqual(const Pixel& pixelA, const Pixel& pixelB) {
        return pixelA.r == pixelB.r && pixelA.g == pixelB.g && pixelA.b == pixelB.b;
    }

    // Custom function to compare two Images pixel by pixel
    bool imagesEqual(const Image& img1, const Image& img2) {
        if (img1.width != img2.width || img1.height != img2.height ||
            img1.max_color_value != img2.max_color_value ||
            img1.pixels.size() != img2.pixels.size()) {
            return false;
        }
        for (size_t i = 0; i < img1.pixels.size(); ++i) {
            if (!pixelsEqual(img1.pixels[i], img2.pixels[i])) {
                return false;
            }
        }
        return true;
    }

    constexpr uint16_t MAX_COLOR_VALUE = 255;
    constexpr int IMAGE_WIDTH = 2;
    constexpr int IMAGE_HEIGHT = 2;

    constexpr uint16_t RED_VALUE = 255;
    constexpr uint16_t BLUE_VALUE = 0;
    constexpr uint16_t GRAY_VALUE = 128;

    void test_basic_compression() {
        Image image;
        image.width = IMAGE_WIDTH;
        image.height = IMAGE_HEIGHT;
        image.max_color_value = MAX_COLOR_VALUE;
        image.pixels = {
            Pixel{.r = RED_VALUE, .g = BLUE_VALUE, .b = BLUE_VALUE},   // Red
            Pixel{.r = BLUE_VALUE, .g = RED_VALUE, .b = BLUE_VALUE},   // Green
            Pixel{.r = BLUE_VALUE, .g = BLUE_VALUE, .b = RED_VALUE},   // Blue
            Pixel{.r = RED_VALUE, .g = RED_VALUE, .b = BLUE_VALUE}     // Yellow
        };

        const CompressedImage compressedImage = compress_soa(image);

        // Check metadata
        assert(compressedImage.width == image.width);
        assert(compressedImage.height == image.height);
        assert(compressedImage.max_color == image.max_color_value);

        // Decompress and verify pixel data
        const Image decompressedImage = decompress_soa(compressedImage);
        assert(imagesEqual(decompressedImage, image));

        std::cout << "test_basic_compression passed.\n";
    }

    void test_duplicate_colors() {
        Image image;
        image.width = IMAGE_WIDTH;
        image.height = IMAGE_HEIGHT;
        image.max_color_value = MAX_COLOR_VALUE;
        image.pixels = {
            Pixel{.r = RED_VALUE, .g = BLUE_VALUE, .b = BLUE_VALUE},   // Red
            Pixel{.r = RED_VALUE, .g = BLUE_VALUE, .b = BLUE_VALUE},   // Red
            Pixel{.r = BLUE_VALUE, .g = RED_VALUE, .b = BLUE_VALUE},   // Green
            Pixel{.r = BLUE_VALUE, .g = RED_VALUE, .b = BLUE_VALUE}    // Green
        };

        const CompressedImage compressedImage = compress_soa(image);

        // Check metadata
        assert(compressedImage.width == image.width);
        assert(compressedImage.height == image.height);
        assert(compressedImage.max_color == image.max_color_value);

        // Decompress and verify pixel data
        const Image decompressedImage = decompress_soa(compressedImage);
        assert(imagesEqual(decompressedImage, image));

        std::cout << "test_duplicate_colors passed.\n";
    }

    void test_single_color_image() {
        Image image;
        image.width = IMAGE_WIDTH;
        image.height = IMAGE_HEIGHT;
        image.max_color_value = MAX_COLOR_VALUE;
        image.pixels = {
            Pixel{.r = GRAY_VALUE, .g = GRAY_VALUE, .b = GRAY_VALUE}, // Gray
            Pixel{.r = GRAY_VALUE, .g = GRAY_VALUE, .b = GRAY_VALUE}, // Gray
            Pixel{.r = GRAY_VALUE, .g = GRAY_VALUE, .b = GRAY_VALUE}, // Gray
            Pixel{.r = GRAY_VALUE, .g = GRAY_VALUE, .b = GRAY_VALUE}  // Gray
        };

        const CompressedImage compressedImage = compress_soa(image);

        // Check metadata
        assert(compressedImage.width == image.width);
        assert(compressedImage.height == image.height);
        assert(compressedImage.max_color == image.max_color_value);

        // Decompress and verify pixel data
        const Image decompressedImage = decompress_soa(compressedImage);
        assert(imagesEqual(decompressedImage, image));

        std::cout << "test_single_color_image passed.\n";
    }
}

int main() {
    test_basic_compression();
    test_duplicate_colors();
    test_single_color_image();

    std::cout << "All tests passed.\n";
    return 0;
}
