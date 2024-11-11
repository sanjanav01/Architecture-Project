#include <gtest/gtest.h>
#include "common/binaryio.hpp"       // Include the read_ppm function/

void validate_cppm_file(const std::string& file_path, const CompressedImage& expected_image) {
    CompressedImage read_image = read_cppm(file_path);

    // Validate header data
    EXPECT_EQ(read_image.width, expected_image.width);
    EXPECT_EQ(read_image.height, expected_image.height);
    EXPECT_EQ(read_image.max_color, expected_image.max_color);

    // Validate color table
    ASSERT_EQ(read_image.color_table.size(), expected_image.color_table.size());
    for (size_t i = 0; i < read_image.color_table.size(); ++i) {
        EXPECT_EQ(read_image.color_table[i], expected_image.color_table[i]);
    }

    // Validate pixel indices
    ASSERT_EQ(read_image.pixel_indices.size(), expected_image.pixel_indices.size());
    for (size_t i = 0; i < read_image.pixel_indices.size(); ++i) {
        EXPECT_EQ(read_image.pixel_indices[i], expected_image.pixel_indices[i]);
    }
}

TEST(WriteCPPMTest, SmallColorTable) {
    CompressedImage image;
    image.width = 2;
    image.height = 2;
    image.max_color = 255;
    image.color_table = { 0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00 };  // Red, Green, Blue, Yellow
    image.pixel_indices = { 0, 1, 2, 3 };  // Each pixel references a unique color

    const std::string file_path = "test_small_color_table.cppm";
    write_cppm(file_path, image);
    validate_cppm_file(file_path, image);
}

TEST(WriteCPPMTest, LargeColorTable) {
    CompressedImage image;
    image.width = 4;
    image.height = 4;
    image.max_color = 65535;

    // Create a color table of 300 unique colors
    for (uint32_t i = 0; i < 300; ++i) {
        image.color_table.push_back(i * 12345);  // Random unique color values
    }

    // Create a sequence of pixel indices, all referring to colors in the color table
    for (int i = 0; i < 16; ++i) {
        image.pixel_indices.push_back(static_cast<unsigned int>(i % 300));
    }

    const std::string file_path = "test_large_color_table.cppm";
    write_cppm(file_path, image);
    validate_cppm_file(file_path, image);
}

TEST(WriteCPPMTest, SmallImageWith2ByteIndices) {
    CompressedImage image;
    image.width = 1;
    image.height = 2;
    image.max_color = 255;

    // Add a color table with 500 colors, requiring 2-byte indices
    for (uint32_t i = 0; i < 500; ++i) {
        image.color_table.push_back(i * 54321);
    }

    // Pixel indices referring to color table entries
    image.pixel_indices = { 499, 200 };  // Using large indices to verify 2-byte index handling

    const std::string file_path = "test_2byte_indices.cppm";
    write_cppm(file_path, image);
    validate_cppm_file(file_path, image);
}
