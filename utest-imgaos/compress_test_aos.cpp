#include <gtest/gtest.h>
#include "../imgaos/imageaos.hpp"
#include "common/image_types.hpp"
#include "common/binaryio.hpp"
#include "helpers/helpers.hpp"
#include <filesystem>
#include <iostream>

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

        image.pixels = {
            Pixel{.r = RED_VALUE, .g = 0, .b = 0},         // Red
            Pixel{.r = 0, .g = GREEN_VALUE, .b = 0},       // Green
            Pixel{.r = 0, .g = 0, .b = BLUE_VALUE},        // Blue
            Pixel{.r = RED_VALUE, .g = GREEN_VALUE, .b = 0} // Yellow
        };

        const CompressedImage compressedImage = compress_aos(image);
        ASSERT_EQ(compressedImage.width, image.width);
        ASSERT_EQ(compressedImage.height, image.height);
        ASSERT_EQ(compressedImage.max_color, image.max_color_value);

        const std::vector<uint32_t> expectedColorTable = {
            encodeColor(RED_VALUE, 0, 0),
            encodeColor(0, GREEN_VALUE, 0),
            encodeColor(0, 0, BLUE_VALUE),
            encodeColor(RED_VALUE, GREEN_VALUE, 0)
        };
        ASSERT_EQ(compressedImage.color_table, expectedColorTable);

        const std::vector<uint32_t> expectedPixelIndices = {0, 1, 2, 3};
        ASSERT_EQ(compressedImage.pixel_indices, expectedPixelIndices);

        std::cout << "test_basic_compression passed.\n";
    }

    void test_duplicate_colors() {
        Image image;
        image.width = IMAGE_WIDTH;
        image.height = IMAGE_HEIGHT;
        image.max_color_value = MAX_COLOR_VALUE;

        image.pixels = {
            Pixel{.r = RED_VALUE, .g = 0, .b = 0},
            Pixel{.r = RED_VALUE, .g = 0, .b = 0},
            Pixel{.r = 0, .g = GREEN_VALUE, .b = 0},
            Pixel{.r = 0, .g = GREEN_VALUE, .b = 0}
        };

        const CompressedImage compressedImage = compress_aos(image);
        ASSERT_EQ(compressedImage.width, image.width);
        ASSERT_EQ(compressedImage.height, image.height);
        ASSERT_EQ(compressedImage.max_color, image.max_color_value);

        const std::vector<uint32_t> expectedColorTable = {
            encodeColor(RED_VALUE, 0, 0),
            encodeColor(0, GREEN_VALUE, 0)
        };
        ASSERT_EQ(compressedImage.color_table, expectedColorTable);

        const std::vector<uint32_t> expectedPixelIndices = {0, 0, 1, 1};
        ASSERT_EQ(compressedImage.pixel_indices, expectedPixelIndices);

        std::cout << "test_duplicate_colors passed.\n";
    }

    void test_single_color_image() {
        Image image;
        image.width = IMAGE_WIDTH;
        image.height = IMAGE_HEIGHT;
        image.max_color_value = MAX_COLOR_VALUE;

        image.pixels = {
            Pixel{.r = GRAY_VALUE, .g = GRAY_VALUE, .b = GRAY_VALUE},
            Pixel{.r = GRAY_VALUE, .g = GRAY_VALUE, .b = GRAY_VALUE},
            Pixel{.r = GRAY_VALUE, .g = GRAY_VALUE, .b = GRAY_VALUE},
            Pixel{.r = GRAY_VALUE, .g = GRAY_VALUE, .b = GRAY_VALUE}
        };

        const CompressedImage compressedImage = compress_aos(image);
        ASSERT_EQ(compressedImage.width, image.width);
        ASSERT_EQ(compressedImage.height, image.height);
        ASSERT_EQ(compressedImage.max_color, image.max_color_value);

        const std::vector<uint32_t> expectedColorTable = {
            encodeColor(GRAY_VALUE, GRAY_VALUE, GRAY_VALUE)
        };
        ASSERT_EQ(compressedImage.color_table, expectedColorTable);

        const std::vector<uint32_t> expectedPixelIndices = {0, 0, 0, 0};
        ASSERT_EQ(compressedImage.pixel_indices, expectedPixelIndices);

        std::cout << "test_single_color_image passed.\n";
    }
}

TEST(CompressTest, BasicCompression) {
    test_basic_compression();
}

TEST(CompressTest, DuplicateColors) {
    test_duplicate_colors();
}

TEST(CompressTest, SingleColorImage) {
    test_single_color_image();
}

TEST(CompressTest, DeerSmall) {
    const std::string inputPath = "/Users/sanjana/Downloads/input/deer-small.ppm";
    const std::string generatedOutputPath = "/Users/sanjana/Architecture-Project/test-outputs/deer-small-output.cppm";
    const std::string decompressedOutputPath = "/Users/sanjana/Architecture-Project/test-outputs/deer-small-decompressed.ppm";

    const Image inputImage = read_ppm(inputPath);
    const CompressedImage compressedImage = compress_aos(inputImage);
    write_cppm(generatedOutputPath, compressedImage);

    const Image decompressedImage = decompress(compressedImage);
    write_ppm(decompressedOutputPath, decompressedImage);

    ASSERT_EQ(inputImage.width, decompressedImage.width);
    ASSERT_EQ(inputImage.height, decompressedImage.height);
    ASSERT_EQ(inputImage.max_color_value, decompressedImage.max_color_value);
    ASSERT_EQ(inputImage.pixels.size(), decompressedImage.pixels.size());

    bool pixels_match = true;
    for (size_t i = 0; i < inputImage.pixels.size(); ++i) {
        if (inputImage.pixels[i].r != decompressedImage.pixels[i].r ||
            inputImage.pixels[i].g != decompressedImage.pixels[i].g ||
            inputImage.pixels[i].b != decompressedImage.pixels[i].b) {
            std::cout << "Pixel mismatch at index " << i
                      << ": Original(R,G,B) = (" << inputImage.pixels[i].r << ", "
                      << inputImage.pixels[i].g << ", " << inputImage.pixels[i].b << ") "
                      << "Decompressed(R,G,B) = (" << decompressedImage.pixels[i].r << ", "
                      << decompressedImage.pixels[i].g << ", " << decompressedImage.pixels[i].b << ")\n";
            pixels_match = false;
            break;
        }
    }
    ASSERT_TRUE(pixels_match) << "Pixels do not match between original and decompressed images.";

    if (pixels_match) {
        ASSERT_TRUE(compareImages(decompressedOutputPath, inputPath)) << "Images differ after decompression in binary comparison";
    }
}

TEST(CompressTest, DeerLarge) {
    const std::string inputPath = "/Users/sanjana/Downloads/input/deer-large.ppm";
    const std::string generatedOutputPath = "/Users/sanjana/Architecture-Project/test-outputs/deer-large-output.cppm";
    const std::string decompressedOutputPath = "/Users/sanjana/Architecture-Project/test-outputs/deer-large-decompressed.ppm";

    const Image inputImage = read_ppm(inputPath);
    const CompressedImage compressedImage = compress_aos(inputImage);
    write_cppm(generatedOutputPath, compressedImage);

    const Image decompressedImage = decompress(compressedImage);
    write_ppm(decompressedOutputPath, decompressedImage);

    ASSERT_EQ(inputImage.width, decompressedImage.width);
    ASSERT_EQ(inputImage.height, decompressedImage.height);
    ASSERT_EQ(inputImage.max_color_value, decompressedImage.max_color_value);
    ASSERT_EQ(inputImage.pixels.size(), decompressedImage.pixels.size());

    bool pixels_match = true;
    for (size_t i = 0; i < inputImage.pixels.size(); ++i) {
        if (inputImage.pixels[i].r != decompressedImage.pixels[i].r ||
            inputImage.pixels[i].g != decompressedImage.pixels[i].g ||
            inputImage.pixels[i].b != decompressedImage.pixels[i].b) {
            std::cout << "Pixel mismatch at index " << i
                      << ": Original(R,G,B) = (" << inputImage.pixels[i].r << ", "
                      << inputImage.pixels[i].g << ", " << inputImage.pixels[i].b << ") "
                      << "Decompressed(R,G,B) = (" << decompressedImage.pixels[i].r << ", "
                      << decompressedImage.pixels[i].g << ", " << decompressedImage.pixels[i].b << ")\n";
            pixels_match = false;
            break;
        }
    }
    ASSERT_TRUE(pixels_match) << "Pixels do not match between original and decompressed images.";

    if (pixels_match) {
        ASSERT_TRUE(compareImages(decompressedOutputPath, inputPath)) << "Images differ after decompression in binary comparison";
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}