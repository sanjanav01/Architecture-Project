// metadata_test.cpp
#include "metadata.hpp"
#include "image_types.hpp"
#include <gtest/gtest.h>

// Define constants for test values
static constexpr int IMAGE_WIDTH = 100;
static constexpr int IMAGE_HEIGHT = 200;
static constexpr int MAX_COLOR_VALUE = 255;

// Test for get_metadata function
TEST(MetadataTest, GetMetadata) {
  // Create an Image object with sample values
  Image image;
  image.width = IMAGE_WIDTH;
  image.height = IMAGE_HEIGHT;
  image.max_color_value = MAX_COLOR_VALUE;
  image.pixels = std::vector<Pixel>(static_cast<size_t>(image.width) * static_cast<size_t>(image.height));  // Initialize pixel data

  // Call get_metadata to extract metadata
  const Metadata metadata = get_metadata(image);

  // Check if the metadata values are correct
  EXPECT_EQ(metadata.width, IMAGE_WIDTH);
  EXPECT_EQ(metadata.height, IMAGE_HEIGHT);
  EXPECT_EQ(metadata.maxColorValue, MAX_COLOR_VALUE);
}

// Test for Metadata::toString function
TEST(MetadataTest, ToString) {
  // Create a Metadata object with sample values
  Metadata metadata{};
  metadata.width = IMAGE_WIDTH;
  metadata.height = IMAGE_HEIGHT;
  metadata.maxColorValue = MAX_COLOR_VALUE;

  // Check if toString returns the correct formatted string
  const std::string expectedOutput = "Width: 100\nHeight: 200\nMax Color Value: 255";
  EXPECT_EQ(metadata.toString(), expectedOutput);
}
