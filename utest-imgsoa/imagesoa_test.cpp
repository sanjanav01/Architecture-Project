// imagesoa_test.cpp

#include <gtest/gtest.h>
#include "imagesoa.hpp"

// Test if `cutfreq` correctly identifies and removes infrequent colors
TEST(ImageSOATest, CutFreq_RemovesInfrequentColors) {
  ImageSOA image(2, 2); // 2x2 image

  // Define colors for each pixel in SOA format
  image.R = {255, 0, 0, 0};  // Red, Green, Green, Blue
  image.G = {0, 255, 255, 0};
  image.B = {0, 0, 0, 255};

  int frequency_threshold = 2;
  image.cutfreq(frequency_threshold);

  // Test that infrequent colors (Red and Blue) have been replaced with Green
  EXPECT_EQ(image.R[0], 0); // Red replaced with Green (closest frequent color)
  EXPECT_EQ(image.G[0], 255);
  EXPECT_EQ(image.B[0], 0);

  EXPECT_EQ(image.R[3], 0); // Blue replaced with Green (closest frequent color)
  EXPECT_EQ(image.G[3], 255);
  EXPECT_EQ(image.B[3], 0);
}

// Test if `cutfreq` does not replace colors when all colors meet the threshold
TEST(ImageSOATest, CutFreq_NoReplacementWhenAllMeetThreshold) {
  ImageSOA image(2, 2); // 2x2 image

  // Define colors for each pixel in SOA format (all pixels Green)
  image.R = {0, 0, 0, 0};
  image.G = {255, 255, 255, 255};
  image.B = {0, 0, 0, 0};

  int frequency_threshold = 2;
  image.cutfreq(frequency_threshold);

  // Ensure all pixels remain Green
  for (size_t i = 0; i < image.R.size(); ++i) {
    EXPECT_EQ(image.R[i], 0);
    EXPECT_EQ(image.G[i], 255);
    EXPECT_EQ(image.B[i], 0);
  }
}
