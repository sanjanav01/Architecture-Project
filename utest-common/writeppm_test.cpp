#include "common/binaryio.hpp"  // Adjust path as needed
#include "common/image_types.hpp"
#include <gtest/gtest.h>
#include <fstream>
#include <vector>
#include <cstdio> // for std::remove

TEST(BinaryIO, WritePPM) {
    // Step 1: Create a small test image
    Image image;
    image.width = 2;
    image.height = 2;
    image.max_color_value = 255;
    image.pixels = {
        {255, 0, 0},    // Red
        {0, 255, 0},    // Green
        {0, 0, 255},    // Blue
        {255, 255, 0}   // Yellow
    };

    // Path to the test file
    const std::string test_file_path = "test_output.ppm";

    // Step 2: Write the image to a file using write_ppm
    write_ppm(test_file_path, image);

    // Step 3: Open the file and verify its contents
    std::ifstream file(test_file_path, std::ios::binary);
    ASSERT_TRUE(file.is_open()) << "Failed to open test file";

    // Read and check the header
    std::string magic_number;
    int width, height, max_val;
    file >> magic_number >> width >> height >> max_val;
    ASSERT_EQ(magic_number, "P6");
    ASSERT_EQ(width, image.width);
    ASSERT_EQ(height, image.height);
    ASSERT_EQ(max_val, image.max_color_value);

    // Consume the newline character after max color value
    file.ignore();

    // Step 4: Verify the pixel data
    for (const auto& pixel : image.pixels) {
        uint8_t r, g, b;
        file.read(reinterpret_cast<char*>(&r), 1);
        file.read(reinterpret_cast<char*>(&g), 1);
        file.read(reinterpret_cast<char*>(&b), 1);
        ASSERT_EQ(r, static_cast<uint8_t>(pixel.r));
        ASSERT_EQ(g, static_cast<uint8_t>(pixel.g));
        ASSERT_EQ(b, static_cast<uint8_t>(pixel.b));
    }

    // Step 5: Check for errors during file reading
    ASSERT_FALSE(file.fail()) << "Error reading pixel data from file";

    file.close();
    std::remove(test_file_path.c_str());
}
