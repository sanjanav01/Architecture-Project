// imagesoa_test.cpp

#include <gtest/gtest.h>
#include "imgsoa/imagesoa.hpp"

// Define a constant for the max color value
static constexpr int MAX_COLOR_VALUE = 255;

// Test if `cutfreq` correctly identifies and removes infrequent colors
TEST(ImageSOATest, CutFreq_RemovesInfrequentColors) {
  ImageSOA image(2, 2); // 2x2 image

  // Define colors for each pixel in SOA format
  image.R = {MAX_COLOR_VALUE, 0, 0, 0};  // Red, Green, Green, Blue
  image.G = {0, MAX_COLOR_VALUE, MAX_COLOR_VALUE, 0};
  image.B = {0, 0, 0, MAX_COLOR_VALUE};

  constexpr int frequency_threshold = 2;
  image.cutfreq(frequency_threshold);

  // Test that infrequent colors (Red and Blue) have been replaced with Green
  EXPECT_EQ(image.R[0], 0); // Red replaced with Green (closest frequent color)
  EXPECT_EQ(image.G[0], MAX_COLOR_VALUE);
  EXPECT_EQ(image.B[0], 0);

  EXPECT_EQ(image.R[3], 0); // Blue replaced with Green (closest frequent color)
  EXPECT_EQ(image.G[3], MAX_COLOR_VALUE);
  EXPECT_EQ(image.B[3], 0);
}

// Test if `cutfreq` does not replace colors when all colors meet the threshold
TEST(ImageSOATest, CutFreq_NoReplacementWhenAllMeetThreshold) {
  ImageSOA image(2, 2); // 2x2 image

  // Define colors for each pixel in SOA format (all pixels Green)
  image.R = {0, 0, 0, 0};
  image.G = {MAX_COLOR_VALUE, MAX_COLOR_VALUE, MAX_COLOR_VALUE, MAX_COLOR_VALUE};
  image.B = {0, 0, 0, 0};

  constexpr int frequency_threshold = 2;
  image.cutfreq(frequency_threshold);

  // Ensure all pixels remain Green
  for (size_t i = 0; i < image.R.size(); ++i) {
    EXPECT_EQ(image.R[i], 0);
    EXPECT_EQ(image.G[i], MAX_COLOR_VALUE);
    EXPECT_EQ(image.B[i], 0);
  }
}

