#include <gtest/gtest.h>
#include "common/image_types.hpp"
#include "common/binaryio.hpp"
#include "imgaos/imageaos.hpp"
#include "helpers.hpp"

constexpr static int new_max_value = 255;

TEST(CompressTest, TestMaxLevel128) { //write the individual test case
  // Define paths to input, expected output, and generated output
  std::string const inputPath = "/Users/sanjana/Downloads/input/deer-small.ppm";
  std::string const expectedOutputPath = "/Users/sanjana/Downloads/outputs/maxlevel/deer-small-255.ppm";
  std::string const generatedOutputPath = "/Users/sanjana/Architecture-Project/test-outputs/deer-small-output-255.cppm";

  // Load and modify the image, save it to generatedOutputPath
  Image inputImage = read_ppm(inputPath);
  CompressedImageAOS compress_aos(inputImage); //takes in imageaos need to change that
  write_ppm(generatedOutputPath, inputImage);  // Write the modified image

  // Use compareImages with cmp to check if the generated image matches the expected output
  ASSERT_TRUE(compareImages(generatedOutputPath, expectedOutputPath)) << "Images differ for maxlevel 128";
}
