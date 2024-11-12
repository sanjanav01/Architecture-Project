#include <gtest/gtest.h>
#include "common/binaryio.hpp"       // Include the read_ppm function

// Define shorter constants for readability and to avoid magic numbers
constexpr static int MAX_COLOR_LARGE = 65535;
constexpr static int COLOR_TABLE_LARGE = 300;
constexpr static int COLOR_MULT_SMALL = 12345;
constexpr static int COLOR_MULT_LARGE = 54321;
constexpr static int PIXEL_COUNT_SMALL = 16;
constexpr static int NUMCOLORS = 500;
constexpr static int WIDTH_SMALL = 2;
constexpr static int HEIGHT_SMALL = 2;
constexpr static int WIDTH_LARGE = 4;
constexpr static int HEIGHT_LARGE = 4;
constexpr static int WIDTH_SINGLE = 1;
constexpr static int HEIGHT_DOUBLE = 2;
constexpr static int IDX_LARGE = 499;
constexpr static int IDX_MEDIUM = 200;
constexpr static int MAGIC = 255;
constexpr static int RED = 0xFF0000;
constexpr static int GREEN = 0x00FF00;
constexpr static int BLUE = 0x0000FF;
constexpr static int YELLOW = 0xFFFF00;
namespace {
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
}

TEST(WriteCPPMTest, SmallColorTable) {
    CompressedImage image;
    image.width = WIDTH_SMALL;
    image.height = HEIGHT_SMALL;
    image.max_color = MAGIC;
    image.color_table = { RED, GREEN, BLUE, YELLOW };  // Red, Green, Blue, Yellow
    image.pixel_indices = { 0, 1, 2, 3 };  // Each pixel references a unique color

    const std::string file_path = "test_small_color_table.cppm";
    write_cppm(file_path, image);
    validate_cppm_file(file_path, image);
}

TEST(WriteCPPMTest, LargeColorTable) {
    CompressedImage image;
    image.width = WIDTH_LARGE;
    image.height = HEIGHT_LARGE;
    image.max_color = MAX_COLOR_LARGE;

    // Create a color table of 300 unique colors
    for (uint32_t i = 0; i < COLOR_TABLE_LARGE; ++i) {
        image.color_table.push_back(i * COLOR_MULT_SMALL);  // Random unique color values
    }

    // Create a sequence of pixel indices, all referring to colors in the color table
    for (int i = 0; i < PIXEL_COUNT_SMALL; ++i) {
        image.pixel_indices.push_back(static_cast<unsigned int>(i % COLOR_TABLE_LARGE));
    }

    const std::string file_path = "test_large_color_table.cppm";
    write_cppm(file_path, image);
    validate_cppm_file(file_path, image);
}

TEST(WriteCPPMTest, SmallImageWith2ByteIndices) {
    CompressedImage image;
    image.width = WIDTH_SINGLE;
    image.height = HEIGHT_DOUBLE;
    image.max_color = MAGIC;

    // Add a color table with 500 colors, requiring 2-byte indices
    for (uint32_t i = 0; i < NUMCOLORS; ++i) {
        image.color_table.push_back(i * COLOR_MULT_LARGE);
    }

    // Pixel indices referring to color table entries
    image.pixel_indices = { IDX_LARGE, IDX_MEDIUM };  // Using large indices to verify 2-byte index handling

    const std::string file_path = "test_2byte_indices.cppm";
    write_cppm(file_path, image);
    validate_cppm_file(file_path, image);
}
