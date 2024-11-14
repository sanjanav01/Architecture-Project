#include <gtest/gtest.h>
#include "imgsoa/imagesoa.hpp"
#include "common/image_types.hpp"
#include "common/binaryio.hpp"
#include "helpers/helpers.hpp"
#include <filesystem>
#include <iostream>

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
}

TEST(CompressTestSOA, BasicCompression) {
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

    ASSERT_EQ(compressedImage.width, image.width);
    ASSERT_EQ(compressedImage.height, image.height);
    ASSERT_EQ(compressedImage.max_color, image.max_color_value);

    const Image decompressedImage = decompress_soa(compressedImage);
    ASSERT_TRUE(imagesEqual(decompressedImage, image));

    std::cout << "test_basic_compression passed.\n";
}

TEST(CompressTestSOA, DuplicateColors) {
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

    ASSERT_EQ(compressedImage.width, image.width);
    ASSERT_EQ(compressedImage.height, image.height);
    ASSERT_EQ(compressedImage.max_color, image.max_color_value);

    const Image decompressedImage = decompress_soa(compressedImage);
    ASSERT_TRUE(imagesEqual(decompressedImage, image));

    std::cout << "test_duplicate_colors passed.\n";
}

TEST(CompressTestSOA, SingleColorImage) {
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

    ASSERT_EQ(compressedImage.width, image.width);
    ASSERT_EQ(compressedImage.height, image.height);
    ASSERT_EQ(compressedImage.max_color, image.max_color_value);

    const Image decompressedImage = decompress_soa(compressedImage);
    ASSERT_TRUE(imagesEqual(decompressedImage, image));

    std::cout << "test_single_color_image passed.\n";
}

TEST(CompressTestSOA, DeerSmall) {
    const std::string inputPath = "/Users/kalyani/Documents/CompArch/Input:Output/input/deer-small.ppm";
    const std::string generatedOutputPath = "/Users/kalyani/Documents/CompArch/Architecture-Project/test-outputs/deer-small-output.cppm";
    const std::string decompressedOutputPath = "/Users/kalyani/Documents/CompArch/Architecture-Project/test-outputs/deer-small-decompressed.ppm";

    const Image inputImage = read_ppm(inputPath);
    const CompressedImage compressedImage = compress_soa(inputImage);
    write_cppm(generatedOutputPath, compressedImage);

    const Image decompressedImage = decompress_soa(compressedImage);
    write_ppm(decompressedOutputPath, decompressedImage);

    ASSERT_EQ(inputImage.width, decompressedImage.width);
    ASSERT_EQ(inputImage.height, decompressedImage.height);
    ASSERT_EQ(inputImage.max_color_value, decompressedImage.max_color_value);
    ASSERT_EQ(inputImage.pixels.size(), decompressedImage.pixels.size());

    bool pixels_match = true;
    for (size_t i = 0; i < inputImage.pixels.size(); ++i) {
        if (!pixelsEqual(inputImage.pixels[i], decompressedImage.pixels[i])) {
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
