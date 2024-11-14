#include <gtest/gtest.h>
#include "common/binaryio.hpp"
#include "imageaos.hpp"
#include "helpers.hpp"
constexpr static int HUND = 100;
constexpr static int THOUS = 1000;
TEST(DeerSmall, TestDeerSmallResize1000) {
    std::string const inputPath = "/Users/shriyasingh/Desktop/Architecture-Project/test_resources/input/deer-small.ppm";
    std::string const expectedOutputPath = "/Users/shriyasingh/Desktop/Architecture-Project/test_resources/outputs/resize 2/deer-small-1000.ppm";
    std::string const generatedOutputPath = "/Users/shriyasingh/Desktop/Architecture-Project/test_resources/test_outputs/deersmall_resized1000.ppm";

    Image const inputImage = read_ppm(inputPath);
    Image const expOutput = read_ppm(expectedOutputPath);
    Image const genOutput = ImageAOS::resize_aos(inputImage, THOUS, THOUS);
    write_ppm(generatedOutputPath, genOutput);

    EXPECT_TRUE(compareImagesByPixel(expOutput, genOutput)) << "Images differ for resize deer small to 1000";
}

TEST(DeerSmall, TestDeerSmallResize100) {
    std::string const inputPath = "/Users/shriyasingh/Desktop/Architecture-Project/test_resources/input/deer-small.ppm";
    std::string const expectedOutputPath = "/Users/shriyasingh/Desktop/Architecture-Project/test_resources/outputs/resize 2/deer-small-100.ppm";
    std::string const generatedOutputPath = "/Users/shriyasingh/Desktop/Architecture-Project/test_resources/test_outputs/deersmall_resized100.ppm";

    Image const inputImage = read_ppm(inputPath);
    Image const expOutput = read_ppm(expectedOutputPath);
    Image const genOutput = ImageAOS::resize_aos(inputImage, HUND, HUND);
    write_ppm(generatedOutputPath, genOutput);

    EXPECT_TRUE(compareImagesByPixel(expOutput, genOutput)) << "Images differ for resize deer small to 100";
}