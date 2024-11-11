// aosresize_test.cpp

#include "imgaos/imageaos.hpp"
#include <gtest/gtest.h>

TEST(ImageAOSResize, SolidColorResize) {
    int width = 4;
    int height = 4;
    int color = 100;
    ImageAOS image(width, height);

    // Fill image with a single color
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            image.pixels[static_cast<size_t>(y) * static_cast<size_t>(width) + static_cast<size_t>(x)] = {color, color, color};
        }
    }

    // Resize image
    ImageAOS resized_image = resize_aos(image, 2, 2);

    // Check resized values
    for (const auto& pixel : resized_image.pixels) {
        EXPECT_EQ(pixel.R, color);
        EXPECT_EQ(pixel.G, color);
        EXPECT_EQ(pixel.B, color);
    }
}

TEST(ImageAOSResize, CheckerboardResize) {
    int width = 4;
    int height = 4;
    ImageAOS image(width, height);

    // Fill image with a checkerboard pattern
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int color = ((x + y) % 2 == 0) ? 255 : 0;
            image.pixels[static_cast<size_t>(y) * static_cast<size_t>(width) + static_cast<size_t>(x)] = {color, color, color};
        }
    }

    // Resize image
    ImageAOS resized_image = resize_aos(image, 2, 2);

    // Check resized values to see if they approximate a checkerboard pattern
    EXPECT_EQ(resized_image.pixels[0].R, 255);
    EXPECT_EQ(resized_image.pixels[0].G, 255);
    EXPECT_EQ(resized_image.pixels[0].B, 255);

    EXPECT_EQ(resized_image.pixels[1].R, 0);
    EXPECT_EQ(resized_image.pixels[1].G, 0);
    EXPECT_EQ(resized_image.pixels[1].B, 0);

    EXPECT_EQ(resized_image.pixels[2].R, 0);
    EXPECT_EQ(resized_image.pixels[2].G, 0);
    EXPECT_EQ(resized_image.pixels[2].B, 0);

    EXPECT_EQ(resized_image.pixels[3].R, 255);
    EXPECT_EQ(resized_image.pixels[3].G, 255);
    EXPECT_EQ(resized_image.pixels[3].B, 255);
}

TEST(ImageAOSResize, LargerResize) {
    int width = 2;
    int height = 2;
    ImageAOS image(width, height);

    // Set up a small 2x2 image with distinct colors
    image.pixels[0] = {255, 0, 0};  // Red
    image.pixels[1] = {0, 255, 0};  // Green
    image.pixels[2] = {0, 0, 255};  // Blue
    image.pixels[3] = {255, 255, 0}; // Yellow

    // Resize image to a larger size
    ImageAOS resized_image = resize_aos(image, 4, 4);

    // Check a few points to see if colors are interpolated
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
