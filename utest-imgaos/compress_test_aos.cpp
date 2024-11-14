#include "../imgaos/imageaos.hpp"
#include <iostream>
#include <cassert>

namespace {
    constexpr uint16_t MAX_COLOR_VALUE = 255;
    constexpr int IMAGE_WIDTH = 2;
    constexpr int IMAGE_HEIGHT = 2;

    constexpr uint16_t RED_VALUE = 255;
    constexpr uint16_t GREEN_VALUE = 255;
    constexpr uint16_t BLUE_VALUE = 255;
    constexpr uint16_t GRAY_VALUE = 128;

    constexpr int RED_SHIFT = 16;
    constexpr int GREEN_SHIFT = 8;

    // Helper function to encode RGB into a uint32_t with bit-shifting
    constexpr uint32_t encodeColor(uint16_t red, uint16_t green, uint16_t blue) {
        return (static_cast<uint32_t>(red) << RED_SHIFT) |
               (static_cast<uint32_t>(green) << GREEN_SHIFT) |
               static_cast<uint32_t>(blue);
    }

    void test_basic_compression() {
        Image image;
        image.width = IMAGE_WIDTH;
        image.height = IMAGE_HEIGHT;
        image.max_color_value = MAX_COLOR_VALUE;

        // Initialize pixels in AoS format
        image.pixels = {
            Pixel{.r = RED_VALUE, .g = 0, .b = 0},         // Red
            Pixel{.r = 0, .g = GREEN_VALUE, .b = 0},       // Green
            Pixel{.r = 0, .g = 0, .b = BLUE_VALUE},        // Blue
            Pixel{.r = RED_VALUE, .g = GREEN_VALUE, .b = 0} // Yellow
        };

        const CompressedImage compressedImage = compress_aos(image);

        // Check metadata
        assert(compressedImage.width == image.width);
        assert(compressedImage.height == image.height);
        assert(compressedImage.max_color == image.max_color_value);

        // Verify the color table size (4 unique colors)
        assert(compressedImage.color_table.size() == 4);

        // Expected color table values using helper function
        const std::vector<uint32_t> expectedColorTable = {
            encodeColor(RED_VALUE, 0, 0),            // Red
            encodeColor(0, GREEN_VALUE, 0),          // Green
            encodeColor(0, 0, BLUE_VALUE),           // Blue
            encodeColor(RED_VALUE, GREEN_VALUE, 0)   // Yellow
        };
        assert(compressedImage.color_table == expectedColorTable);

        const std::vector<uint32_t> expectedPixelIndices = {0, 1, 2, 3};
        assert(compressedImage.pixel_indices == expectedPixelIndices);

        std::cout << "test_basic_compression passed.\n";
    }

    void test_duplicate_colors() {
        Image image;
        image.width = IMAGE_WIDTH;
        image.height = IMAGE_HEIGHT;
        image.max_color_value = MAX_COLOR_VALUE;

        // Initialize duplicate colors in AoS format
        image.pixels = {
            Pixel{.r = RED_VALUE, .g = 0, .b = 0},         // Red
            Pixel{.r = RED_VALUE, .g = 0, .b = 0},         // Red
            Pixel{.r = 0, .g = GREEN_VALUE, .b = 0},       // Green
            Pixel{.r = 0, .g = GREEN_VALUE, .b = 0}        // Green
        };

        const CompressedImage compressedImage = compress_aos(image);

        // Check metadata
        assert(compressedImage.width == image.width);
        assert(compressedImage.height == image.height);
        assert(compressedImage.max_color == image.max_color_value);

        // Verify the color table size (2 unique colors)
        assert(compressedImage.color_table.size() == 2);

        const std::vector<uint32_t> expectedColorTable = {
            encodeColor(RED_VALUE, 0, 0),          // Red
            encodeColor(0, GREEN_VALUE, 0)         // Green
        };
        assert(compressedImage.color_table == expectedColorTable);

        const std::vector<uint32_t> expectedPixelIndices = {0, 0, 1, 1};
        assert(compressedImage.pixel_indices == expectedPixelIndices);

        std::cout << "test_duplicate_colors passed.\n";
    }

    void test_single_color_image() {
        Image image;
        image.width = IMAGE_WIDTH;
        image.height = IMAGE_HEIGHT;
        image.max_color_value = MAX_COLOR_VALUE;

        // Initialize a single color in AoS format
        image.pixels = {
            Pixel{.r = GRAY_VALUE, .g = GRAY_VALUE, .b = GRAY_VALUE},
            Pixel{.r = GRAY_VALUE, .g = GRAY_VALUE, .b = GRAY_VALUE},
            Pixel{.r = GRAY_VALUE, .g = GRAY_VALUE, .b = GRAY_VALUE},
            Pixel{.r = GRAY_VALUE, .g = GRAY_VALUE, .b = GRAY_VALUE}
        };

        const CompressedImage compressedImage = compress_aos(image);

        // Check metadata
        assert(compressedImage.width == image.width);
        assert(compressedImage.height == image.height);
        assert(compressedImage.max_color == image.max_color_value);

        // Verify that there is only one color in the color table
        assert(compressedImage.color_table.size() == 1);

        const std::vector<uint32_t> expectedColorTable = {
            encodeColor(GRAY_VALUE, GRAY_VALUE, GRAY_VALUE)
        };
        assert(compressedImage.color_table == expectedColorTable);

        const std::vector<uint32_t> expectedPixelIndices = {0, 0, 0, 0};
        assert(compressedImage.pixel_indices == expectedPixelIndices);

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
