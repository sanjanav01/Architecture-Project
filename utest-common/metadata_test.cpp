// metadata_test.cpp
#include "metadata.hpp"
#include "image_types.hpp"
#include <gtest/gtest.h>

// Test for get_metadata function
TEST(MetadataTest, GetMetadata) {
    // Create an Image object with sample values
    Image image;
    image.width = 100;
    image.height = 200;
    image.max_color_value = 255;
    image.pixels = std::vector<Pixel>(static_cast<size_t>(image.width) * static_cast<size_t>(image.height));  // Initialize pixel data

    // Call get_metadata to extract metadata
    Metadata metadata = get_metadata(image);

    // Check if the metadata values are correct
    EXPECT_EQ(metadata.width, 100);
    EXPECT_EQ(metadata.height, 200);
    EXPECT_EQ(metadata.maxColorValue, 255);
}

// Test for Metadata::toString function
TEST(MetadataTest, ToString) {
    // Create a Metadata object with sample values
    Metadata metadata;
    metadata.width = 100;
    metadata.height = 200;
    metadata.maxColorValue = 255;

    // Check if toString returns the correct formatted string
    std::string expectedOutput = "Width: 100\nHeight: 200\nMax Color Value: 255";
    EXPECT_EQ(metadata.toString(), expectedOutput);
}