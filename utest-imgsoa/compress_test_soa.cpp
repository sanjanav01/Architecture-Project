#include "../imgsoa/imagesoa.hpp"
#include <iostream>
#include <cassert>

namespace {
    constexpr uint16_t MAX_COLOR_VALUE = 255;
    constexpr int IMAGE_WIDTH = 2;
    constexpr int IMAGE_HEIGHT = 2;

    constexpr uint16_t RED_VALUE = 255;
    constexpr uint16_t BLUE_VALUE = 0;
    constexpr uint16_t GRAY_VALUE = 128;

    void test_basic_compression() {
        ImageSOA image;
        image.width = IMAGE_WIDTH;
        image.height = IMAGE_HEIGHT;
        image.maxColorValue = MAX_COLOR_VALUE;

        // Initialize colors in SOA format
        image.red = { RED_VALUE, BLUE_VALUE, BLUE_VALUE, RED_VALUE };
        image.green = { BLUE_VALUE, RED_VALUE, BLUE_VALUE, RED_VALUE };
        image.blue = { BLUE_VALUE, BLUE_VALUE, RED_VALUE, BLUE_VALUE };

        const CompressedImageSOA compressedImage = compress_soa(image);

        // Check metadata
        assert(compressedImage.magicNumber == "C6");
        assert(compressedImage.width == image.width);
        assert(compressedImage.height == image.height);
        assert(compressedImage.maxColorValue == image.maxColorValue);
        assert(compressedImage.colorTableSize == 4);

        // Verify the color table values (flattened RGB)
        const std::vector<uint16_t> expectedColorTable = {
            RED_VALUE, BLUE_VALUE, BLUE_VALUE,    // Red
            BLUE_VALUE, RED_VALUE, BLUE_VALUE,    // Green
            BLUE_VALUE, BLUE_VALUE, RED_VALUE,    // Blue
            RED_VALUE, RED_VALUE, BLUE_VALUE      // Yellow
        };
        assert(compressedImage.colorTable == expectedColorTable);

        // Verify the pixel indices (they should map to the colors in the color table)
        const std::vector<uint32_t> expectedPixelIndices = {0, 1, 2, 3};
        assert(compressedImage.pixelIndices == expectedPixelIndices);

        std::cout << "test_basic_compression passed.\n";
    }

    void test_duplicate_colors() {
        ImageSOA image;
        image.width = IMAGE_WIDTH;
        image.height = IMAGE_HEIGHT;
        image.maxColorValue = MAX_COLOR_VALUE;

        // Initialize duplicate colors in SOA format
        image.red = { RED_VALUE, RED_VALUE, BLUE_VALUE, BLUE_VALUE };
        image.green = { BLUE_VALUE, BLUE_VALUE, RED_VALUE, RED_VALUE };
        image.blue = { BLUE_VALUE, BLUE_VALUE, BLUE_VALUE, BLUE_VALUE };

        const CompressedImageSOA compressedImage = compress_soa(image);

        // Check metadata
        assert(compressedImage.magicNumber == "C6");
        assert(compressedImage.width == image.width);
        assert(compressedImage.height == image.height);
        assert(compressedImage.maxColorValue == image.maxColorValue);
        assert(compressedImage.colorTableSize == 2);

        // Verify the color table values (flattened RGB)
        const std::vector<uint16_t> expectedColorTable = {
            RED_VALUE, BLUE_VALUE, BLUE_VALUE,    // Red
            BLUE_VALUE, RED_VALUE, BLUE_VALUE     // Green
        };
        assert(compressedImage.colorTable == expectedColorTable);

        // Verify the pixel indices (should reference only two unique colors)
        const std::vector<uint32_t> expectedPixelIndices = {0, 0, 1, 1};
        assert(compressedImage.pixelIndices == expectedPixelIndices);

        std::cout << "test_duplicate_colors passed.\n";
    }

    void test_single_color_image() {
        ImageSOA image;
        image.width = IMAGE_WIDTH;
        image.height = IMAGE_HEIGHT;
        image.maxColorValue = MAX_COLOR_VALUE;

        // Initialize a single color in SOA format
        image.red = { GRAY_VALUE, GRAY_VALUE, GRAY_VALUE, GRAY_VALUE };
        image.green = { GRAY_VALUE, GRAY_VALUE, GRAY_VALUE, GRAY_VALUE };
        image.blue = { GRAY_VALUE, GRAY_VALUE, GRAY_VALUE, GRAY_VALUE };

        const CompressedImageSOA compressedImage = compress_soa(image);

        // Check metadata
        assert(compressedImage.magicNumber == "C6");
        assert(compressedImage.width == image.width);
        assert(compressedImage.height == image.height);
        assert(compressedImage.maxColorValue == image.maxColorValue);
        assert(compressedImage.colorTableSize == 1);

        // Verify the color table values (flattened RGB)
        const std::vector<uint16_t> expectedColorTable = {GRAY_VALUE, GRAY_VALUE, GRAY_VALUE}; // Gray
        assert(compressedImage.colorTable == expectedColorTable);

        // Verify the pixel indices (should all reference the single color)
        const std::vector<uint32_t> expectedPixelIndices = {0, 0, 0, 0};
        assert(compressedImage.pixelIndices == expectedPixelIndices);

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
