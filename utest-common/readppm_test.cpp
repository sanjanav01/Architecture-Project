#include <fstream>
#include <gtest/gtest.h>
#include "common/binaryio.hpp"       // Include the read_ppm function
#include <string>

TEST(ReadPPMTest, ValidPPMFile) {
    // ppm2.ppm
    std::ofstream file("test_resources", std::ios::binary);

    // PPM Header
    file << "P6\n2 2\n255\n";

    // Pixel data: R, G, B values for each pixel in binary format
    std::vector<uint8_t> pixel_data = {
        255, 0, 0,     // Red
        0, 255, 0,     // Green
        0, 0, 255,     // Blue
        255, 255, 0    // Yellow
    };

    file.write(reinterpret_cast<char*>(pixel_data.data()), static_cast<std::streamsize>(pixel_data.size()));

    file.close();


    std::string file_path = "test_resources";

    Image image = read_ppm(file_path);

    EXPECT_EQ(image.width, 2);
    EXPECT_EQ(image.height, 2);
    EXPECT_EQ(image.max_color_value, 255);

    ASSERT_EQ(image.pixels.size(), 4);

    EXPECT_EQ(image.pixels[0].r, 255); EXPECT_EQ(image.pixels[0].g, 0); EXPECT_EQ(image.pixels[0].b, 0);
    EXPECT_EQ(image.pixels[1].r, 0);   EXPECT_EQ(image.pixels[1].g, 255); EXPECT_EQ(image.pixels[1].b, 0);
    EXPECT_EQ(image.pixels[2].r, 0);   EXPECT_EQ(image.pixels[2].g, 0);   EXPECT_EQ(image.pixels[2].b, 255);
    EXPECT_EQ(image.pixels[3].r, 255); EXPECT_EQ(image.pixels[3].g, 255); EXPECT_EQ(image.pixels[3].b, 0);
}

TEST(ReadPPMTest, InvalidMagicNumber) {
    std::string file_path = "test_resources/resources/invalid_magic.ppm";
    EXPECT_THROW(read_ppm(file_path), std::runtime_error);
}
