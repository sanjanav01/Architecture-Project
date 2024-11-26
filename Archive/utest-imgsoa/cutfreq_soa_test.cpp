#include <gtest/gtest.h>
#include "imgsoa/imagesoa.hpp"

static constexpr int MAX_COLOR_VALUE = 255;

TEST(ImageSOATest, CutFreq_RemovesInfrequentColors) {
  ImageSOA image(Width{2}, Height{2}, MaxColorValue{MAX_COLOR_VALUE});

  image.R = {MAX_COLOR_VALUE, 0, 0, 0};
  image.G = {0, MAX_COLOR_VALUE, MAX_COLOR_VALUE, 0};
  image.B = {0, 0, 0, MAX_COLOR_VALUE};

  constexpr int frequency_threshold = 2;
  image.cutfreq(frequency_threshold);

  EXPECT_EQ(image.R[0], 0);
  EXPECT_EQ(image.G[0], MAX_COLOR_VALUE);
  EXPECT_EQ(image.B[0], 0);

  EXPECT_EQ(image.R[3], 0);
  EXPECT_EQ(image.G[3], MAX_COLOR_VALUE);
  EXPECT_EQ(image.B[3], 0);
}


TEST(ImageSOATest, CutFreq_NoReplacementWhenAllMeetThreshold) {
  ImageSOA image(Width{2}, Height{2}, MaxColorValue{MAX_COLOR_VALUE});

  image.R = {0, 0, 0, 0};
  image.G = {MAX_COLOR_VALUE, MAX_COLOR_VALUE, MAX_COLOR_VALUE, MAX_COLOR_VALUE};
  image.B = {0, 0, 0, 0};

  constexpr int frequency_threshold = 2;
  image.cutfreq(frequency_threshold);

  for (size_t i = 0; i < image.R.size(); ++i) {
    EXPECT_EQ(image.R[i], 0);
    EXPECT_EQ(image.G[i], MAX_COLOR_VALUE);
    EXPECT_EQ(image.B[i], 0);
  }
}