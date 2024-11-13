#include <gtest/gtest.h>
#include "common/image_types.hpp"
#include "common/binaryio.hpp"
#include "imageaos.hpp"
#include "helpers.hpp"

#include <chrono> //for testing execution time

constexpr static int new_max_value100 = 100;
constexpr static int new_max_value1000 = 1000;
constexpr static int new_max_value1 = 1;
constexpr static int new_max_value128 = 128;
constexpr static int new_max_value512 = 512;
constexpr static int new_max_value255 = 255;
constexpr static int new_max_value65535 = 65535;
constexpr static double execution_time = 11.0;


TEST(MaxLevelTest, TestDeerMaxLevel100) {
    // Define paths to input, expected output, and generated output
    std::string const inputPath = "/Users/sanjana/Downloads/input/deer-small.ppm";
    std::string const expectedOutputPath = "/Users/sanjana/Downloads/out/deer-small-100.ppm";
    std::string const generatedOutputPath = "/Users/sanjana/Architecture-Project/test-outputs/deer-small-output-100.ppm";

    // Load the input image, apply maxlevel, and save the generated output
    Image inputImage = read_ppm(inputPath);
    ImageAOS::maxlevel(inputImage, new_max_value100);
    write_ppm(generatedOutputPath, inputImage);  // Write the modified image

    // Reload generated and expected images to compare them pixel-by-pixel
    Image generatedImage = read_ppm(generatedOutputPath);
    Image expectedImage = read_ppm(expectedOutputPath);

    // Ensure dimensions are the same
    ASSERT_EQ(generatedImage.width, expectedImage.width) << "Image widths do not match";
    ASSERT_EQ(generatedImage.height, expectedImage.height) << "Image heights do not match";

    // Flag for tracking if there are any mismatched pixels
    bool allPixelsMatch = true;

    // Compare each pixel, and print details if there is a mismatch
    for (size_t rowIdx = 0; rowIdx < static_cast<size_t>(generatedImage.height); ++rowIdx) {
        for (size_t colIdx = 0; colIdx < static_cast<size_t>(generatedImage.width); ++colIdx) {
            size_t const pixelIndex = (rowIdx * static_cast<size_t>(generatedImage.width)) + colIdx;
            Pixel const& generatedPixel = generatedImage.pixels[pixelIndex];
            Pixel const& expectedPixel = expectedImage.pixels[pixelIndex];

            if (generatedPixel.r != expectedPixel.r ||
                generatedPixel.g != expectedPixel.g ||
                generatedPixel.b != expectedPixel.b) {
                allPixelsMatch = false;
                // std::cout << "Mismatch at pixel (" << colIdx << ", " << rowIdx << "):\n";
                // std::cout << "  Generated - R: " << generatedPixel.r
                //           << ", G: " << generatedPixel.g
                //           << ", B: " << generatedPixel.b << "\n";
                // std::cout << "  Expected  - R: " << expectedPixel.r
                //           << ", G: " << expectedPixel.g
                //           << ", B: " << expectedPixel.b << "\n";
            }
        }
    }

    // Assert that all pixels matched; if not, the detailed mismatch info will have been printed
    ASSERT_TRUE(allPixelsMatch) << "Some pixels did not match for maxlevel 100";
}

TEST(MaxLevelTest, TestDeerMaxLevel1000) {
  // Define paths to input, expected output, and generated output
  std::string const inputPath = "/Users/sanjana/Downloads/input/deer-small.ppm";
  std::string const expectedOutputPath = "/Users/sanjana/Downloads/out/deer-small-1000.ppm";
  std::string const generatedOutputPath = "/Users/sanjana/Architecture-Project/test-outputs/deer-small-output-1000.ppm";

  // Load and modify the image, save it to generatedOutputPath
  Image inputImage = read_ppm(inputPath);
  ImageAOS::maxlevel(inputImage, new_max_value1000);
  write_ppm(generatedOutputPath, inputImage);  // Write the modified image

  // Use compareImages with cmp to check if the generated image matches the expected output
  ASSERT_TRUE(compareImages(generatedOutputPath, expectedOutputPath)) << "Images differ for maxlevel 1000";
}

TEST(MaxLevelTest, TestDeerMaxLevel255) {
  // Define paths to input, expected output, and generated output
  std::string const inputPath = "/Users/sanjana/Downloads/input/deer-small.ppm";
  std::string const expectedOutputPath = "/Users/sanjana/Downloads/outputs/maxlevel/deer-small-255.ppm";
  std::string const generatedOutputPath = "/Users/sanjana/Architecture-Project/test-outputs/deer-small-output-255.ppm";

  // Load and modify the image, save it to generatedOutputPath
  Image inputImage = read_ppm(inputPath);
  ImageAOS::maxlevel(inputImage, new_max_value255);
  write_ppm(generatedOutputPath, inputImage);  // Write the modified image

  // Use compareImages with cmp to check if the generated image matches the expected output
  ASSERT_TRUE(compareImages(generatedOutputPath, expectedOutputPath)) << "Images differ for maxlevel 255";
}

TEST(MaxLevelTest, TestLakeMaxLevel255) {
  // Define paths to input, expected output, and generated output
  std::string const inputPath = "/Users/sanjana/Downloads/input/lake-small.ppm";
  std::string const expectedOutputPath = "/Users/sanjana/Downloads/outputs/maxlevel/lake-small-255.ppm";
  std::string const generatedOutputPath = "/Users/sanjana/Architecture-Project/test-outputs/lake-small-output-255.ppm";

  // Load and modify the image, save it to generatedOutputPath
  Image inputImage = read_ppm(inputPath);
  ImageAOS::maxlevel(inputImage, new_max_value255);
  write_ppm(generatedOutputPath, inputImage);  // Write the modified image

  // Use compareImages with cmp to check if the generated image matches the expected output
  ASSERT_TRUE(compareImages(generatedOutputPath, expectedOutputPath)) << "Images differ for maxlevel 255";
}

TEST(MaxLevelTest, TestDeerMaxLevel65535) {
  // Define paths to input, expected output, and generated output
  std::string const inputPath = "/Users/sanjana/Downloads/input/deer-small.ppm";
  std::string const expectedOutputPath = "/Users/sanjana/Downloads/outputs/maxlevel/deer-small-65535.ppm";
  std::string const generatedOutputPath = "/Users/sanjana/Architecture-Project/test-outputs/deer-small-output-65535.ppm";

  // Load and modify the image, save it to generatedOutputPath
  Image inputImage = read_ppm(inputPath);
  ImageAOS::maxlevel(inputImage, new_max_value65535);
  write_ppm(generatedOutputPath, inputImage);  // Write the modified image

  // Use compareImages with cmp to check if the generated image matches the expected output
  ASSERT_TRUE(compareImages(generatedOutputPath, expectedOutputPath)) << "Images differ for maxlevel 65535";
}

TEST(MaxLevelTest, TestLakeMaxLevel65535) {
  // Define paths to input, expected output, and generated output
  std::string const inputPath = "/Users/sanjana/Downloads/input/lake-small.ppm";
  std::string const expectedOutputPath = "/Users/sanjana/Downloads/outputs/maxlevel/lake-small-65535.ppm";
  std::string const generatedOutputPath = "/Users/sanjana/Architecture-Project/test-outputs/lake-small-output-65535.ppm";

  // Load and modify the image, save it to generatedOutputPath
  Image inputImage = read_ppm(inputPath);
  ImageAOS::maxlevel(inputImage, new_max_value65535);
  write_ppm(generatedOutputPath, inputImage);  // Write the modified image

  // Use compareImages with cmp to check if the generated image matches the expected output
  ASSERT_TRUE(compareImages(generatedOutputPath, expectedOutputPath)) << "Images differ for maxlevel 65535";
}

TEST(MaxLevelPerformanceTest, LakeLargeMaxLevel65535) {
  // Load the large input image
  std::string const inputPath = "/Users/sanjana/Downloads/input/lake-large.ppm";
  Image inputImage = read_ppm(inputPath);

  // Set the max level to 65535
  int const new_max_value = new_max_value65535;

  // Measure the start time
  auto start = std::chrono::high_resolution_clock::now();

  // Perform the maxlevel operation
  ImageAOS::maxlevel(inputImage, new_max_value);

  // Measure the end time
  auto end = std::chrono::high_resolution_clock::now();

  // Calculate the duration in seconds
  std::chrono::duration<double> const duration = end - start;
  double const elapsed_time = duration.count();

  // Check if the operation completes within the expected time threshold
  constexpr double max_allowed_time = execution_time; // seconds
  ASSERT_LE(elapsed_time, max_allowed_time) << "Execution time exceeded for lake-large.ppm with maxlevel 65535";

  std::cout << "Execution time for maxlevel: " << elapsed_time << " seconds\n";
}

namespace {
  void validateScaledPixels(const Image& originalImage, const Image& scaledImage, int new_max_value) {
    double const scaling_factor = static_cast<double>(new_max_value) / originalImage.max_color_value;

    for (size_t i = 0; i < originalImage.pixels.size(); ++i) {
      auto const& originalPixel = originalImage.pixels[i];
      auto const& scaledPixel = scaledImage.pixels[i];

      // Calculate the expected scaled values
      int expectedR = static_cast<int>(std::floor(originalPixel.r * scaling_factor));
      int expectedG = static_cast<int>(std::floor(originalPixel.g * scaling_factor));
      int expectedB = static_cast<int>(std::floor(originalPixel.b * scaling_factor));

      expectedR = std::clamp(expectedR, 0, new_max_value);
      expectedG = std::clamp(expectedG, 0, new_max_value);
      expectedB = std::clamp(expectedB, 0, new_max_value);

      // Validate that the scaled pixel values match the expected values
      ASSERT_EQ(scaledPixel.r, expectedR) << "Red channel mismatch at pixel " << i;
      ASSERT_EQ(scaledPixel.g, expectedG) << "Green channel mismatch at pixel " << i;
      ASSERT_EQ(scaledPixel.b, expectedB) << "Blue channel mismatch at pixel " << i;
    }
  }

}

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

TEST(MaxLevelTest, TestBoundaryCaseMaxLevel65535) {
  std::string const inputPath = "/Users/sanjana/Downloads/input/deer-small.ppm";
  Image const originalImage = read_ppm(inputPath);

  Image scaledImage = originalImage;
  ImageAOS::maxlevel(scaledImage, new_max_value65535);

  validateScaledPixels(originalImage, scaledImage, new_max_value65535);
}

