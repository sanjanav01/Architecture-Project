#include <gtest/gtest.h>
#include "common/image_types.hpp"
#include "common/binaryio.hpp"
#include "imageaos.hpp"
#include "helpers.hpp"

constexpr static int new_max_value100 = 100;
constexpr static int new_max_value1000 = 1000;
constexpr static int new_max_value255 = 255;
constexpr static int new_max_value65535 = 65535;


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