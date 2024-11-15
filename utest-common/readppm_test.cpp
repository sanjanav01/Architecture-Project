#include <fstream>
#include <gtest/gtest.h>
#include "common/binaryio.hpp"
#include <string>
#include <vector>
#include <cstdint>

constexpr static int MAGIC = 255;
constexpr static int NUM = 123;

TEST(ReadPPMTest, ValidPPMFile) {
    std::ofstream file("valid_test.ppm", std::ios::binary);
    file << "P6\n2 2\n255\n";
    std::vector<uint8_t> pixel_data = {MAGIC, 0, 0, 0, MAGIC, 0, 0, 0, MAGIC, MAGIC, MAGIC, 0};
    file.write(reinterpret_cast<char*>(pixel_data.data()), static_cast<std::streamsize>(pixel_data.size()));
    file.close();

    Image image = read_ppm("valid_test.ppm");
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
    std::ofstream file("invalid_magic.ppm");
    file << "P5\n2 2\n255\n";
    std::vector<uint8_t> pixel_data = {MAGIC, 0, 0, 0, MAGIC, 0, 0, 0, MAGIC, MAGIC, MAGIC, 0};
    file.write(reinterpret_cast<char*>(pixel_data.data()), static_cast<std::streamsize>(pixel_data.size()));
    file.close();

    EXPECT_THROW(read_ppm("invalid_magic.ppm"), std::runtime_error);
}

TEST(ReadPPMTest, InvalidDimensions) {
    std::ofstream file("invalid_dimensions.ppm");
    file << "P6\n-1 2\n255\n";
    std::vector<uint8_t> pixel_data = {MAGIC, 0, 0, 0, MAGIC, 0, 0, 0, MAGIC, MAGIC, MAGIC, 0};
    file.write(reinterpret_cast<char*>(pixel_data.data()), static_cast<std::streamsize>(pixel_data.size()));

    file.close();

    EXPECT_THROW(read_ppm("invalid_dimensions.ppm"), std::runtime_error);
}

TEST(ReadPPMTest, InvalidMaxColorValue) {
    std::ofstream file("invalid_maxcolor.ppm");
    file << "P6\n2 2\n500\n";
    std::vector<uint8_t> pixel_data = {MAGIC, 0, 0, 0, MAGIC, 0, 0, 0, MAGIC, MAGIC, MAGIC, 0};
    file.write(reinterpret_cast<char*>(pixel_data.data()), static_cast<std::streamsize>(pixel_data.size()));
    file.close();

    EXPECT_THROW(read_ppm("invalid_maxcolor.ppm"), std::runtime_error);
}

TEST(ReadPPMTest, ExtraDataAfterImage) {
    std::ofstream file("extra_data.ppm", std::ios::binary);
    file << "P6\n2 2\n255\n";
    std::vector<uint8_t> pixel_data = {MAGIC, 0, 0, 0, MAGIC, 0, 0, 0, MAGIC, MAGIC, MAGIC, 0, NUM, NUM, NUM};
    file.write(reinterpret_cast<char*>(pixel_data.data()), static_cast<std::streamsize>(pixel_data.size()));
    file.close();

    EXPECT_NO_THROW({
        Image image = read_ppm("extra_data.ppm");
        EXPECT_EQ(image.width, 2);
        EXPECT_EQ(image.height, 2);
        EXPECT_EQ(image.max_color_value, 255);
        ASSERT_EQ(image.pixels.size(), 4);
        EXPECT_EQ(image.pixels[0].r, 255); EXPECT_EQ(image.pixels[0].g, 0); EXPECT_EQ(image.pixels[0].b, 0);
        EXPECT_EQ(image.pixels[1].r, 0);   EXPECT_EQ(image.pixels[1].g, 255); EXPECT_EQ(image.pixels[1].b, 0);
        EXPECT_EQ(image.pixels[2].r, 0);   EXPECT_EQ(image.pixels[2].g, 0);   EXPECT_EQ(image.pixels[2].b, 255);
        EXPECT_EQ(image.pixels[3].r, 255); EXPECT_EQ(image.pixels[3].g, 255); EXPECT_EQ(image.pixels[3].b, 0);
    });
}

TEST(ReadPPMTest, InvalidPixelDataLength) {
    std::ofstream file("invalid_pixel_data.ppm", std::ios::binary);
    file << "P6\n2 2\n255\n";
    std::vector<uint8_t> pixel_data = {MAGIC, 0, 0, 0, MAGIC};  // Incomplete pixel data
    file.write(reinterpret_cast<char*>(pixel_data.data()), static_cast<std::streamsize>(pixel_data.size()));
    file.close();

    EXPECT_THROW(read_ppm("invalid_pixel_data.ppm"), std::runtime_error);
}
