#include "common/binaryio.hpp"  // Adjust path as needed
#include "common/image_types.hpp"
#include <gtest/gtest.h>
#include <fstream>
#include <vector>
#include <cstdio> // for std::remove
constexpr static int MaxByteValue = 255;

TEST(BinaryIO, WritePPM) {

    // Step 1: Create a small test image
    Image image;
    image.width = 2;
    image.height = 2;
    image.max_color_value = MaxByteValue;
    image.pixels = {
        {.r=MaxByteValue, .g=0, .b=0},    // Red
        {.r=0, .g=MaxByteValue, .b=0},    // Green
        {.r=0, .g=0, .b=MaxByteValue},    // Blue
        {.r=MaxByteValue, .g=MaxByteValue, .b=0}   // Yellow
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
    int width = 0;
    int height = 0;
    int max_val = 0;
    file >> magic_number >> width >> height >> max_val;
    ASSERT_EQ(magic_number, "P6");
    ASSERT_EQ(width, image.width);
    ASSERT_EQ(height, image.height);
    ASSERT_EQ(max_val, image.max_color_value);

    // Consume the newline character after max color value
    file.ignore();

    // Step 4: Verify the pixel data
    for (const auto& pixel : image.pixels) {
        uint8_t red = 0;
        uint8_t green = 0;
        uint8_t blue = 0;
        file.read(reinterpret_cast<char*>(&red), 1);
        file.read(reinterpret_cast<char*>(&green), 1);
        file.read(reinterpret_cast<char*>(&blue), 1);
        ASSERT_EQ(red, static_cast<uint8_t>(pixel.r));
        ASSERT_EQ(green, static_cast<uint8_t>(pixel.g));
        ASSERT_EQ(blue, static_cast<uint8_t>(pixel.b));
    }

    // Step 5: Check for errors during file reading
    ASSERT_FALSE(file.fail()) << "Error reading pixel data from file";

    file.close();
    std::remove(test_file_path.c_str());
}
