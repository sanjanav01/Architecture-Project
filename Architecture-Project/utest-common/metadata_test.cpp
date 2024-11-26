// metadata_test.cpp
#include "metadata.hpp"
#include "image_types.hpp"
#include <gtest/gtest.h>

constexpr static int HUND = 100;
constexpr static int TWOHUND = 200;
constexpr static int MAGICNUM = 255;

// Test for get_metadata function
TEST(MetadataTest, GetMetadata) {
    // Create an Image object with sample values
    Image image;
    image.width = HUND;
    image.height = TWOHUND;
    image.max_color_value = MAGICNUM;
    image.pixels = std::vector<Pixel>(static_cast<size_t>(image.width) * static_cast<size_t>(image.height));  // Initialize pixel data

    // Call get_metadata to extract metadata
    const auto [width, height, maxColorValue] = get_metadata(image);

    // Check if the metadata values are correct
    EXPECT_EQ(width, 100);
    EXPECT_EQ(height, 200);
    EXPECT_EQ(maxColorValue, 255);
}

// Test for Metadata::toString function
TEST(MetadataTest, ToString) {
    // Create a Metadata object with sample values
    Metadata metadata{};
    metadata.width = HUND;
    metadata.height = TWOHUND;
    metadata.maxColorValue = MAGICNUM;

    // Check if toString returns the correct formatted string
    std::string const expectedOutput = "Width: 100\nHeight: 200\nMax Color Value: 255";
    EXPECT_EQ(metadata.toString(), expectedOutput);
}