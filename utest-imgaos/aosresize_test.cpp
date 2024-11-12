// aosresize_test.cpp

#include "imgaos/imageaos.hpp"
#include <gtest/gtest.h>

constexpr static int MAGIC = 255;

TEST(ImageAOSResize, SolidColorResize) {
    constexpr int width = 4;
    constexpr int height = 4;
    constexpr int color = 100;
    ImageAOS image(width, height);

    // Fill image with a single color
    for (int hgt = 0; hgt < height; ++hgt) {
        for (int wdt = 0; wdt < width; ++wdt) {
            image.pixels[(static_cast<size_t>(hgt) * static_cast<size_t>(width)) + static_cast<size_t>(wdt)] = {.R=color, .G=color, .B=color};
        }
    }

    // Check resized values
    for (ImageAOS const resized_image = resize_aos(image, 2, 2); const auto& pixel : resized_image.pixels) {
        EXPECT_EQ(pixel.R, color);
        EXPECT_EQ(pixel.G, color);
        EXPECT_EQ(pixel.B, color);
    }
}

// Helper function to check if a pixel has the expected color values
namespace {
    void CheckPixelColor(const Pixel& pixel, int expected_red, int expected_green, int expected_blue) {
        EXPECT_EQ(pixel.R, expected_red);
        EXPECT_EQ(pixel.G, expected_green);
        EXPECT_EQ(pixel.B, expected_blue);
    }
}

// Simplified test function
TEST(ImageAOSResize, CheckerboardResize) {
    constexpr int original_width = 4;
    constexpr int original_height = 4;
    constexpr int resized_width = 2;
    constexpr int resized_height = 2;

    ImageAOS image(original_width, original_height);

    // Fill image with a checkerboard pattern
    for (int row = 0; row < original_height; ++row) {
        for (int col = 0; col < original_width; ++col) {
            int const color = ((col + row) % 2 == 0) ? 255 : 0;
            image.pixels[(static_cast<size_t>(row) * static_cast<size_t>(original_width)) + static_cast<size_t>(col)] = {.R=color, .G=color, .B=color};
        }
    }

    // Resize image
    const ImageAOS resized_image = resize_aos(image, resized_width, resized_height);

    // Check resized values to see if they approximate a checkerboard pattern
    CheckPixelColor(resized_image.pixels[0], MAGIC, MAGIC, MAGIC);
    CheckPixelColor(resized_image.pixels[1], 0, 0, 0);
    CheckPixelColor(resized_image.pixels[2], 0, 0, 0);
    CheckPixelColor(resized_image.pixels[3], MAGIC, MAGIC, MAGIC);
}


TEST(ImageAOSResize, LargerResize) {
    constexpr int width = 2;
    constexpr int height = 2;
    ImageAOS image(width, height);
    image.pixels[0] = {.R=MAGIC, .G=0, .B=0};  // Red
    image.pixels[1] = {.R=0, .G=MAGIC, .B=0};  // Green
    image.pixels[2] = {.R=0, .G=0, .B=MAGIC};  // Blue
    image.pixels[3] = {.R=MAGIC, .G=MAGIC, .B=0}; // Yellow
    const ImageAOS resized_image = resize_aos(image, 4, 4);
    EXPECT_NEAR(resized_image.pixels[0].R, 255, 1);
    EXPECT_NEAR(resized_image.pixels[0].G, 0, 1);
    EXPECT_NEAR(resized_image.pixels[0].B, 0, 1);
    EXPECT_NEAR(resized_image.pixels[3].R, 0, 1);
    EXPECT_NEAR(resized_image.pixels[3].G, 255, 1);
    EXPECT_NEAR(resized_image.pixels[3].B, 0, 1);
    EXPECT_NEAR(resized_image.pixels[12].R, 0, 1);
    EXPECT_NEAR(resized_image.pixels[12].G, 0, 1);
    EXPECT_NEAR(resized_image.pixels[12].B, 255, 1);
    EXPECT_NEAR(resized_image.pixels[15].R, 255, 1);
    EXPECT_NEAR(resized_image.pixels[15].G, 255, 1);
    EXPECT_NEAR(resized_image.pixels[15].B, 0, 1);
}