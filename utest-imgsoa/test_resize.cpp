#include "imagesoa.hpp"
#include <gtest/gtest.h>

TEST(ImageSOAResize, SolidColorResize) {
    constexpr int width = 4;
    constexpr int height = 4;
    constexpr uint8_t color = 100;
    ImageSOA image(width, height);
    for (size_t i = 0; i < image.R.size(); ++i) {
        image.R.at(i) = color;
        image.G.at(i) = color;
        image.B.at(i) = color;
    }
    const ImageSOA resized_image = resize_soa(image, 2, 2);
    for (size_t i = 0; i < resized_image.R.size(); ++i) {
        EXPECT_EQ(resized_image.R.at(i), color);
        EXPECT_EQ(resized_image.G.at(i), color);
        EXPECT_EQ(resized_image.B.at(i), color);
    }
}

TEST(ImageSOAResize, CheckerboardResize) {
    constexpr int original_width = 4;
    constexpr int original_height = 4;
    ImageSOA image(original_width, original_height);
    for (int ogh = 0; ogh < original_height; ++ogh) {
        for (int ogw = 0; ogw < original_width; ++ogw) {
            const size_t index = (static_cast<size_t>(ogh) * static_cast<size_t>(original_width)) + static_cast<size_t>(ogw);
            uint8_t const color = ((ogw + ogh) % 2 == 0) ? 255 : 0;
            image.R.at(index) = color;
            image.G.at(index) = color;
            image.B.at(index) = color;
        }
    }
    const ImageSOA resized_image = resize_soa(image, 2, 2);
    const std::vector<uint8_t> expected_values = {191, 64, 64, 191};
    for (size_t i = 0; i < resized_image.R.size(); ++i) {
        EXPECT_NEAR(resized_image.R.at(i), expected_values[i], 10);
        EXPECT_NEAR(resized_image.G.at(i), expected_values[i], 10);
        EXPECT_NEAR(resized_image.B.at(i), expected_values[i], 10);
    }
}