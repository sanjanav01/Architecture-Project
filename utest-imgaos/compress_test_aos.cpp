#include "../imgaos/imageaos.hpp"
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
        // Create a simple ImageAOS instance with a 2x2 image of distinct colors
        ImageAOS image;
        image.width = IMAGE_WIDTH;
        image.height = IMAGE_HEIGHT;
        image.maxColorValue = MAX_COLOR_VALUE;
        image.pixels = {
            { .red = RED_VALUE, .green = BLUE_VALUE, .blue = BLUE_VALUE },   // Red
            { .red = BLUE_VALUE, .green = RED_VALUE, .blue = BLUE_VALUE },   // Green
            { .red = BLUE_VALUE, .green = BLUE_VALUE, .blue = RED_VALUE },   // Blue
            { .red = RED_VALUE, .green = RED_VALUE, .blue = BLUE_VALUE }     // Yellow
        };

        const CompressedImageAOS compressedImage = compress_aos(image);

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
        // Create an image where some pixels have the same color
        ImageAOS image;
        image.width = IMAGE_WIDTH;
        image.height = IMAGE_HEIGHT;
        image.maxColorValue = MAX_COLOR_VALUE;
        image.pixels = {
            { .red = RED_VALUE, .green = BLUE_VALUE, .blue = BLUE_VALUE },   // Red
            { .red = RED_VALUE, .green = BLUE_VALUE, .blue = BLUE_VALUE },   // Red
            { .red = BLUE_VALUE, .green = RED_VALUE, .blue = BLUE_VALUE },   // Green
            { .red = BLUE_VALUE, .green = RED_VALUE, .blue = BLUE_VALUE }    // Green
        };

        const CompressedImageAOS compressedImage = compress_aos(image);

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
        // Create an image where all pixels are the same color
        ImageAOS image;
        image.width = IMAGE_WIDTH;
        image.height = IMAGE_HEIGHT;
        image.maxColorValue = MAX_COLOR_VALUE;
        image.pixels = {
            { .red = GRAY_VALUE, .green = GRAY_VALUE, .blue = GRAY_VALUE }, // Gray
            { .red = GRAY_VALUE, .green = GRAY_VALUE, .blue = GRAY_VALUE }, // Gray
            { .red = GRAY_VALUE, .green = GRAY_VALUE, .blue = GRAY_VALUE }, // Gray
            { .red = GRAY_VALUE, .green = GRAY_VALUE, .blue = GRAY_VALUE }  // Gray
        };

        const CompressedImageAOS compressedImage = compress_aos(image);

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
