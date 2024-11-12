#include <fstream>
#include <gtest/gtest.h>
#include "imgsoa/imagesoa.hpp"

constexpr static int SIX = 6;
constexpr static int NINE = 9;
constexpr static int HUND = 100;
constexpr static int HUNDFIFTY = 150;
constexpr static int TWOHUND = 200;

namespace {
    ImageSOA createCheckerboardImage(const int width, const int height) {
        ImageSOA image(width, height);
        size_t const total_pixels = static_cast<size_t>(width) * static_cast<size_t>(height);
        image.R.resize(total_pixels);
        image.G.resize(total_pixels);
        image.B.resize(total_pixels);

        for (int hgt = 0; hgt < height; ++hgt) {
            for (int wdt = 0; wdt < width; ++wdt) {
                int const color = ((wdt + hgt) % 2) * 255;
                const auto index = (static_cast<size_t>(hgt) * static_cast<size_t>(width)) + static_cast<size_t>(wdt);
                image.R[index] = color;
                image.G[index] = color;
                image.B[index] = color;
            }
        }
        return image;
    }
}

// Test resizing from small to larger size (upscale)
TEST(ImageSOAResizeTest, UpscaleCheckerboard) {
    ImageSOA const image = createCheckerboardImage(2, 2);
    const ImageSOA resized_image = image.resize_soa(4, 4);

    EXPECT_EQ(resized_image.width, 4);
    EXPECT_EQ(resized_image.height, 4);

    // Check some pixels to verify interpolation
    EXPECT_EQ(resized_image.R[0], 0); // Top-left corner
    EXPECT_EQ(resized_image.R[3], 255); // Top-right corner
    EXPECT_EQ(resized_image.R[4], 127); // Interpolated value
    EXPECT_EQ(resized_image.R[15], 255); // Bottom-right corner
}

// Test resizing from larger to smaller size (downscale)
TEST(ImageSOAResizeTest, DownscaleCheckerboard) {
    ImageSOA const image = createCheckerboardImage(4, 4);
    const ImageSOA resized_image = image.resize_soa(2, 2);

    EXPECT_EQ(resized_image.width, 2);
    EXPECT_EQ(resized_image.height, 2);

    // Check that the pattern still resembles a checkerboard
    EXPECT_EQ(resized_image.R[0], 0);
    EXPECT_EQ(resized_image.R[1], 255);
    EXPECT_EQ(resized_image.R[2], 255);
    EXPECT_EQ(resized_image.R[3], 0);
}

// Test identity resizing (no size change)
TEST(ImageSOAResizeTest, IdentityResize) {
    const ImageSOA image = createCheckerboardImage(3, 3);
    const ImageSOA resized_image = image.resize_soa(3, 3);

    EXPECT_EQ(resized_image.width, 3);
    EXPECT_EQ(resized_image.height, 3);

    // Verify all pixels match the original image
    for (size_t i = 0; i < image.R.size(); ++i) {
        EXPECT_EQ(resized_image.R[i], image.R[i]);
        EXPECT_EQ(resized_image.G[i], image.G[i]);
        EXPECT_EQ(resized_image.B[i], image.B[i]);
    }
}

// Test non-square resizing
TEST(ImageSOAResizeTest, NonSquareResize) {
    ImageSOA const image = createCheckerboardImage(2, 3);
    const ImageSOA resized_image = image.resize_soa(4, 6);

    EXPECT_EQ(resized_image.width, 4);
    EXPECT_EQ(resized_image.height, 6);

    // Spot-check some values to confirm interpolation correctness
    EXPECT_EQ(resized_image.R[0], 0);
    EXPECT_EQ(resized_image.R[3], 255);
    EXPECT_EQ(resized_image.R[4], 127); // Interpolated
    EXPECT_EQ(resized_image.R[23], 255);
}

// Test resizing a solid color image (uniform value)
TEST(ImageSOAResizeTest, SolidColorResize) {
    ImageSOA image(3, 3);
    image.R.assign(static_cast<size_t>(NINE), HUND);
    image.G.assign(static_cast<size_t>(NINE), HUNDFIFTY);
    image.B.assign(static_cast<size_t>(NINE), TWOHUND);

    ImageSOA resized_image = image.resize_soa(SIX, SIX);

    EXPECT_EQ(resized_image.width, 6);
    EXPECT_EQ(resized_image.height, 6);

    // All pixels in the resized image should match the original color
    for (size_t i = 0; i < static_cast<size_t>(resized_image.width) * static_cast<size_t>(resized_image.height); ++i) {
        EXPECT_EQ(resized_image.R[i], 100);
        EXPECT_EQ(resized_image.G[i], 150);
        EXPECT_EQ(resized_image.B[i], 200);
    }
}
