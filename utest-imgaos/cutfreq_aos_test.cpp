// imageaos_test.cpp

#include <gtest/gtest.h>
#include "imgaos/imageaos.hpp"

// Define constants for color values
static constexpr int MAX_COLOR_VALUE = 255;

// Test if `cutfreq` correctly identifies and removes infrequent colors
TEST(ImageAOSTest, CutFreq_RemovesInfrequentColors) {
  ImageAOS image(2, 2); // 2x2 image

  // Define colors for each pixel
  image.pixels[0] = Pixel{.R = MAX_COLOR_VALUE, .G = 0, .B = 0}; // Red
  image.pixels[1] = Pixel{.R = 0, .G = MAX_COLOR_VALUE, .B = 0}; // Green
  image.pixels[2] = Pixel{.R = 0, .G = MAX_COLOR_VALUE, .B = 0}; // Green
  image.pixels[3] = Pixel{.R = 0, .G = 0, .B = MAX_COLOR_VALUE}; // Blue

  constexpr int frequency_threshold = 2;
  image.cutfreq(frequency_threshold);

  // Test that infrequent colors (Red and Blue) have been replaced
  EXPECT_EQ(image.pixels[0].R, 0); // Red replaced with Green (closest frequent color)
  EXPECT_EQ(image.pixels[0].G, MAX_COLOR_VALUE);
  EXPECT_EQ(image.pixels[0].B, 0);

  EXPECT_EQ(image.pixels[3].R, 0); // Blue replaced with Green (closest frequent color)
  EXPECT_EQ(image.pixels[3].G, MAX_COLOR_VALUE);
  EXPECT_EQ(image.pixels[3].B, 0);
}

// Test if `cutfreq` does not replace colors when all colors meet the threshold
TEST(ImageAOSTest, CutFreq_NoReplacementWhenAllMeetThreshold) {
  ImageAOS image(2, 2); // 2x2 image

  // Define colors for each pixel
  image.pixels[0] = Pixel{.R = 0, .G = MAX_COLOR_VALUE, .B = 0}; // Green
  image.pixels[1] = Pixel{.R = 0, .G = MAX_COLOR_VALUE, .B = 0}; // Green
  image.pixels[2] = Pixel{.R = 0, .G = MAX_COLOR_VALUE, .B = 0}; // Green
  image.pixels[3] = Pixel{.R = 0, .G = MAX_COLOR_VALUE, .B = 0}; // Green

  constexpr int frequency_threshold = 2;
  image.cutfreq(frequency_threshold);

  // Ensure all pixels remain Green
  for (const auto& pixel : image.pixels) {
    EXPECT_EQ(pixel.R, 0);
    EXPECT_EQ(pixel.G, MAX_COLOR_VALUE);
    EXPECT_EQ(pixel.B, 0);
  }
}

