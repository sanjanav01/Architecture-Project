#include <gtest/gtest.h>
#include "common/image_types.hpp"
#include "common/binaryio.hpp"
#include "imageaos.hpp"
#include "helpers.hpp"
#include <chrono> // For measuring execution time

// Constants for test cases
constexpr static int new_max_value100 = 100;
constexpr static int new_max_value1000 = 1000;
constexpr static int new_max_value1 = 1;
constexpr static int new_max_value128 = 128;
constexpr static int new_max_value512 = 512;
constexpr static int new_max_value255 = 255;
constexpr static int new_max_value65535 = 65535;
constexpr static double execution_time = 11.0;

// Utility function for pixel-level validation
namespace {
    void validateScaledPixels(const Image& originalImage, const Image& scaledImage, int new_max_value) {
        ASSERT_EQ(originalImage.width, scaledImage.width) << "Image widths do not match.";
        ASSERT_EQ(originalImage.height, scaledImage.height) << "Image heights do not match.";

        double const scaling_factor = static_cast<double>(new_max_value) / originalImage.max_color_value;

        for (size_t i = 0; i < originalImage.pixels.size(); ++i) {
            auto const& originalPixel = originalImage.pixels[i];
            auto const& scaledPixel = scaledImage.pixels[i];

            // Calculate the expected scaled values
            int const expectedR = std::clamp(static_cast<int>(std::floor(originalPixel.r * scaling_factor)), 0, new_max_value);
            int const expectedG = std::clamp(static_cast<int>(std::floor(originalPixel.g * scaling_factor)), 0, new_max_value);
            int const expectedB = std::clamp(static_cast<int>(std::floor(originalPixel.b * scaling_factor)), 0, new_max_value);

            // Validate each color channel
            ASSERT_EQ(scaledPixel.r, expectedR) << "Red channel mismatch at pixel " << i;
            ASSERT_EQ(scaledPixel.g, expectedG) << "Green channel mismatch at pixel " << i;
            ASSERT_EQ(scaledPixel.b, expectedB) << "Blue channel mismatch at pixel " << i;
        }
    }
}

// Test Cases
TEST(MaxLevelTest, TestDeerMaxLevel100) {
    std::string const inputPath = "/Users/sanjana/Downloads/input/deer-small.ppm";
    Image const originalImage = read_ppm(inputPath);

    Image scaledImage = originalImage;
    ImageAOS::maxlevel(scaledImage, new_max_value100);

    validateScaledPixels(originalImage, scaledImage, new_max_value100);
}

TEST(MaxLevelTest, TestDeerMaxLevel1000) {
    std::string const inputPath = "/Users/sanjana/Downloads/input/deer-small.ppm";
    Image const originalImage = read_ppm(inputPath);

    Image scaledImage = originalImage;
    ImageAOS::maxlevel(scaledImage, new_max_value1000);

    validateScaledPixels(originalImage, scaledImage, new_max_value1000);
}

TEST(MaxLevelTest, TestDeerMaxLevel255) {
    std::string const inputPath = "/Users/sanjana/Downloads/input/deer-small.ppm";
    Image const originalImage = read_ppm(inputPath);

    Image scaledImage = originalImage;
    ImageAOS::maxlevel(scaledImage, new_max_value255);

    validateScaledPixels(originalImage, scaledImage, new_max_value255);
}

TEST(MaxLevelTest, TestLakeMaxLevel255) {
    std::string const inputPath = "/Users/sanjana/Downloads/input/lake-small.ppm";
    Image const originalImage = read_ppm(inputPath);

    Image scaledImage = originalImage;
    ImageAOS::maxlevel(scaledImage, new_max_value255);

    validateScaledPixels(originalImage, scaledImage, new_max_value255);
}

TEST(MaxLevelTest, TestDeerMaxLevel65535) {
    std::string const inputPath = "/Users/sanjana/Downloads/input/deer-small.ppm";
    Image const originalImage = read_ppm(inputPath);

    Image scaledImage = originalImage;
    ImageAOS::maxlevel(scaledImage, new_max_value65535);

    validateScaledPixels(originalImage, scaledImage, new_max_value65535);
}

TEST(MaxLevelTest, TestLakeMaxLevel65535) {
    std::string const inputPath = "/Users/sanjana/Downloads/input/lake-small.ppm";
    Image const originalImage = read_ppm(inputPath);

    Image scaledImage = originalImage;
    ImageAOS::maxlevel(scaledImage, new_max_value65535);

    validateScaledPixels(originalImage, scaledImage, new_max_value65535);
}

TEST(MaxLevelPerformanceTest, LakeLargeMaxLevel65535) {
    std::string const inputPath = "/Users/sanjana/Downloads/input/lake-large.ppm";
    Image inputImage = read_ppm(inputPath);

    auto start = std::chrono::high_resolution_clock::now();
    ImageAOS::maxlevel(inputImage, new_max_value65535);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> const duration = end - start;
    double const elapsed_time = duration.count();

    ASSERT_LE(elapsed_time, execution_time)
        << "Execution time exceeded for lake-large.ppm with maxlevel 65535";

    std::cout << "Execution time for maxlevel: " << elapsed_time << " seconds\n";
}

// Additional Edge Cases
TEST(MaxLevelTest, TestEdgeCaseMaxLevel1) {
    std::string const inputPath = "/Users/sanjana/Downloads/input/deer-small.ppm";
    Image const originalImage = read_ppm(inputPath);

    Image scaledImage = originalImage;
    ImageAOS::maxlevel(scaledImage, new_max_value1);

    validateScaledPixels(originalImage, scaledImage, new_max_value1);
}

TEST(MaxLevelTest, TestIntermediateCaseMaxLevel128) {
    std::string const inputPath = "/Users/sanjana/Downloads/input/deer-small.ppm";
    Image const originalImage = read_ppm(inputPath);

    Image scaledImage = originalImage;
    ImageAOS::maxlevel(scaledImage, new_max_value128);

    validateScaledPixels(originalImage, scaledImage, new_max_value128);
}

TEST(MaxLevelTest, TestBoundaryCaseMaxLevel512) {
    std::string const inputPath = "/Users/sanjana/Downloads/input/deer-small.ppm";
    Image const originalImage = read_ppm(inputPath);

    Image scaledImage = originalImage;
    ImageAOS::maxlevel(scaledImage, new_max_value512);

    validateScaledPixels(originalImage, scaledImage, new_max_value512);
}