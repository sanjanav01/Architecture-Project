// aosresize_test.cpp

#include "imgaos/imageaos.hpp"
#include <gtest/gtest.h>

constexpr static int MAGIC = 255;

TEST(ImageAOSResize, SolidColorResize) {
    constexpr int width = 4;
    constexpr int height = 4;
    constexpr int color = 100;
    Image image(width, height);

    // Fill image with a single color
    for (int hgt = 0; hgt < height; ++hgt) {
        for (int wdt = 0; wdt < width; ++wdt) {
            image.pixels[(static_cast<size_t>(hgt) * static_cast<size_t>(width)) + static_cast<size_t>(wdt)] = {.r=color, .g=color, .b=color};
        }
    }

    // Check resized values
    for (Image const resized_image = ImageAOS::resize_aos(image, 2, 2); const auto& pixel : resized_image.pixels) {
        EXPECT_EQ(pixel.r, color);
        EXPECT_EQ(pixel.g, color);
        EXPECT_EQ(pixel.b, color);
    }
}

// Helper function to check if a pixel has the expected color values
namespace {
    void CheckPixelColor(const Pixel& pixel, int expected_red, int expected_green, int expected_blue) {
        EXPECT_EQ(pixel.r, expected_red);
        EXPECT_EQ(pixel.g, expected_green);
        EXPECT_EQ(pixel.b, expected_blue);
    }
}

// Simplified test function
TEST(ImageAOSResize, CheckerboardResize) {
    constexpr int original_width = 4;
    constexpr int original_height = 4;
    constexpr int resized_width = 2;
    constexpr int resized_height = 2;

    Image image(original_width, original_height);

    // Fill image with a checkerboard pattern
    for (int row = 0; row < original_height; ++row) {
        for (int col = 0; col < original_width; ++col) {
            int const color = ((col + row) % 2 == 0) ? 255 : 0;
            image.pixels[(static_cast<size_t>(row) * static_cast<size_t>(original_width)) + static_cast<size_t>(col)] = {.r = static_cast<uint16_t>(color),.g = static_cast<uint16_t>(color),.b = static_cast<uint16_t>(color)};
        }
    }

    // Resize image
    Image resized_image = ImageAOS::resize_aos(image, resized_width, resized_height);

    // Check resized values to see if they approximate a checkerboard pattern
    CheckPixelColor(resized_image.pixels[0], MAGIC, MAGIC, MAGIC);
    CheckPixelColor(resized_image.pixels[1], 0, 0, 0);
    CheckPixelColor(resized_image.pixels[2], 0, 0, 0);
    CheckPixelColor(resized_image.pixels[3], MAGIC, MAGIC, MAGIC);
}


TEST(ImageAOSResize, LargerResize) {
    constexpr int width = 2;
    constexpr int height = 2;
    Image image(width, height);
    image.pixels[0] = {.r=MAGIC, .g=0, .b=0};  // Red
    image.pixels[1] = {.r=0, .g=MAGIC, .b=0};  // Green
    image.pixels[2] = {.r=0, .g=0, .b=MAGIC};  // Blue
    image.pixels[3] = {.r=MAGIC, .g=MAGIC, .b=0}; // Yellow
    Image resized_image = ImageAOS::resize_aos(image, 4, 4);
    EXPECT_NEAR(resized_image.pixels[0].r, 255, 1);
    EXPECT_NEAR(resized_image.pixels[0].g, 0, 1);
    EXPECT_NEAR(resized_image.pixels[0].b, 0, 1);
    EXPECT_NEAR(resized_image.pixels[3].r, 0, 1);
    EXPECT_NEAR(resized_image.pixels[3].g, 255, 1);
    EXPECT_NEAR(resized_image.pixels[3].b, 0, 1);
    EXPECT_NEAR(resized_image.pixels[12].r, 0, 1);
    EXPECT_NEAR(resized_image.pixels[12].g, 0, 1);
    EXPECT_NEAR(resized_image.pixels[12].b, 255, 1);
    EXPECT_NEAR(resized_image.pixels[15].r, 255, 1);
    EXPECT_NEAR(resized_image.pixels[15].g, 255, 1);
    EXPECT_NEAR(resized_image.pixels[15].b, 0, 1);
}
