// imageaos_test.cpp
/*
#include <gtest/gtest.h>
#include "imgaos/imageaos.hpp"

// Define constants for color values
static constexpr int MAX_COLOR_VALUE = 255;

// Test if `cutfreq` correctly identifies and removes infrequent colors
TEST(ImageAOSTest, CutFreq_RemovesInfrequentColors) {
  ImageAOS image(2, 2); // 2x2 image

  // Define colors for each pixel
  image.pixels[0] = Pixel{.r = MAX_COLOR_VALUE, .g = 0, .b = 0}; // Red
  image.pixels[1] = Pixel{.r = 0, .g = MAX_COLOR_VALUE, .b = 0}; // Green
  image.pixels[2] = Pixel{.r = 0, .g = MAX_COLOR_VALUE, .b = 0}; // Green
  image.pixels[3] = Pixel{.r = 0, .g = 0, .b = MAX_COLOR_VALUE}; // Blue

  constexpr int frequency_threshold = 2;
  image.cutfreq(frequency_threshold);

  // Test that infrequent colors (Red and Blue) have been replaced
  EXPECT_EQ(image.pixels[0].r, 0); // Red replaced with Green (closest frequent color)
  EXPECT_EQ(image.pixels[0].g, MAX_COLOR_VALUE);
  EXPECT_EQ(image.pixels[0].b, 0);

  EXPECT_EQ(image.pixels[3].r, 0); // Blue replaced with Green (closest frequent color)
  EXPECT_EQ(image.pixels[3].g, MAX_COLOR_VALUE);
  EXPECT_EQ(image.pixels[3].b, 0);
}

// Test if `cutfreq` does not replace colors when all colors meet the threshold
TEST(ImageAOSTest, CutFreq_NoReplacementWhenAllMeetThreshold) {
  ImageAOS image(2, 2); // 2x2 image

  // Define colors for each pixel
  image.pixels[0] = Pixel{.r = 0, .g = MAX_COLOR_VALUE, .b = 0}; // Green
  image.pixels[1] = Pixel{.r = 0, .g = MAX_COLOR_VALUE, .b = 0}; // Green
  image.pixels[2] = Pixel{.r = 0, .g = MAX_COLOR_VALUE, .b = 0}; // Green
  image.pixels[3] = Pixel{.r = 0, .g = MAX_COLOR_VALUE, .b = 0}; // Green

  constexpr int frequency_threshold = 2;
  image.cutfreq(frequency_threshold);

  // Ensure all pixels remain Green
  for (const auto& pixel : image.pixels) {
    EXPECT_EQ(pixel.r, 0);
    EXPECT_EQ(pixel.g, MAX_COLOR_VALUE);
    EXPECT_EQ(pixel.b, 0);
  }
}
*/