#include <gtest/gtest.h>
#include "imgaos/imageaos.hpp"

static constexpr int MAX_COLOR_VALUE = 255;

TEST(ImageAOSTest, CutFreq_RemovesInfrequentColors) {
  ImageAOS image(2, 2);

  image.pixels[0] = Pixel{.r = MAX_COLOR_VALUE, .g = 0, .b = 0};
  image.pixels[1] = Pixel{.r = 0, .g = MAX_COLOR_VALUE, .b = 0};
  image.pixels[2] = Pixel{.r = 0, .g = MAX_COLOR_VALUE, .b = 0};
  image.pixels[3] = Pixel{.r = 0, .g = 0, .b = MAX_COLOR_VALUE};

  constexpr int frequency_threshold = 2;
  image.cutfreq(frequency_threshold);

  EXPECT_EQ(image.pixels[0].r, 0);
  EXPECT_EQ(image.pixels[0].g, MAX_COLOR_VALUE);
  EXPECT_EQ(image.pixels[0].b, 0);

  EXPECT_EQ(image.pixels[3].r, 0);
  EXPECT_EQ(image.pixels[3].g, MAX_COLOR_VALUE);
  EXPECT_EQ(image.pixels[3].b, 0);
}

TEST(ImageAOSTest, CutFreq_NoReplacementWhenAllMeetThreshold) {
  ImageAOS image(2, 2);

  image.pixels[0] = Pixel{.r = 0, .g = MAX_COLOR_VALUE, .b = 0};
  image.pixels[1] = Pixel{.r = 0, .g = MAX_COLOR_VALUE, .b = 0};
  image.pixels[2] = Pixel{.r = 0, .g = MAX_COLOR_VALUE, .b = 0};
  image.pixels[3] = Pixel{.r = 0, .g = MAX_COLOR_VALUE, .b = 0};

  constexpr int frequency_threshold = 2;
  image.cutfreq(frequency_threshold);

  for (const auto& pixel : image.pixels) {
    EXPECT_EQ(pixel.r, 0);
    EXPECT_EQ(pixel.g, MAX_COLOR_VALUE);
    EXPECT_EQ(pixel.b, 0);
  }
}

