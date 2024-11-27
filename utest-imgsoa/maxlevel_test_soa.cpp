#include <gtest/gtest.h>
#include "common/image_types.hpp"
#include "common/binaryio.hpp"
#include "imgsoa/imagesoa.hpp"
#include "helpers.hpp"
#include <chrono> // For execution time measurement
#include <cstdlib> // For std::system

// Constants for test cases
constexpr static int new_max_value100 = 100;
constexpr static int new_max_value1000 = 1000;
constexpr static int new_max_value255 = 255;
constexpr static int new_max_value65535 = 65535;
constexpr static double execution_time = 11.0;

TEST(MaxLevelTestSOA, TestDeerMaxLevel100) {
    std::string const inputPath = "/Users/sanjana/Architecture-Project/test_resources/input/deer-small.ppm";
    std::string const expectedOutputPath = "/Users/sanjana/Architecture-Project/test_resources/outputs/maxlevel/deer-small-100.ppm";

    Image const originalImage = read_ppm(inputPath);
    ImageSOA soa = convertToSOA(originalImage);
    soa.maxlevel(new_max_value100);

    Image const scaledImage = convertToImage(soa);
    Image const expectedImage = read_ppm(expectedOutputPath);

    ASSERT_TRUE(compareImagesByPixel(scaledImage, expectedImage))
        << "Images differ for maxlevel 100.";
}

TEST(MaxLevelTestSOA, TestDeerMaxLevel1000) {
    std::string const inputPath = "/Users/sanjana/Downloads/input/deer-small.ppm";
    std::string const expectedOutputPath = "/Users/sanjana/Downloads/out/deer-small-1000.ppm";

    Image const originalImage = read_ppm(inputPath);
    ImageSOA soa = convertToSOA(originalImage);
    soa.maxlevel(new_max_value1000);

    Image const scaledImage = convertToImage(soa);
    Image const expectedImage = read_ppm(expectedOutputPath);

    ASSERT_TRUE(compareImagesByPixel(scaledImage, expectedImage))
        << "Images differ for maxlevel 1000.";
}

TEST(MaxLevelTestSOA, TestDeerMaxLevel255) {
    std::string const inputPath = "/Users/sanjana/Downloads/input/deer-small.ppm";
    std::string const expectedOutputPath = "/Users/sanjana/Downloads/outputs/maxlevel/deer-small-255.ppm";

    Image const originalImage = read_ppm(inputPath);
    ImageSOA soa = convertToSOA(originalImage);
    soa.maxlevel(new_max_value255);

    Image const scaledImage = convertToImage(soa);
    Image const expectedImage = read_ppm(expectedOutputPath);

    ASSERT_TRUE(compareImagesByPixel(scaledImage, expectedImage))
        << "Images differ for maxlevel 255.";
}

TEST(MaxLevelTestSOA, TestDeerMaxLevel65535) {
    std::string const inputPath = "/Users/sanjana/Downloads/input/deer-small.ppm";
    std::string const expectedOutputPath = "/Users/sanjana/Downloads/outputs/maxlevel/deer-small-65535.ppm";

    Image const originalImage = read_ppm(inputPath);
    ImageSOA soa = convertToSOA(originalImage);
    soa.maxlevel(new_max_value65535);

    Image const scaledImage = convertToImage(soa);
    Image const expectedImage = read_ppm(expectedOutputPath);

    ASSERT_TRUE(compareImagesByPixel(scaledImage, expectedImage))
        << "Images differ for maxlevel 65535.";
}

TEST(MaxLevelPerformanceTestSOA, LakeLargeMaxLevel65535) {
    std::string const inputPath = "/Users/sanjana/Architecture-Project/test_resources/input/lake-large.ppm";

    Image const originalImage = read_ppm(inputPath);
    ImageSOA soa = convertToSOA(originalImage);

    auto start = std::chrono::high_resolution_clock::now();
    soa.maxlevel(new_max_value65535);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> const duration = end - start;
    double const elapsed_time = duration.count();

    ASSERT_LE(elapsed_time, execution_time)
        << "Execution time exceeded for lake-large.ppm with maxlevel 65535";

    std::cout << "Execution time for maxlevel: " << elapsed_time << " seconds\n";
}

TEST(MaxLevelTestSOA, TestLakeMaxLevel65535) {
    std::string const inputPath = "//Users/sanjana/Architecture-Project/test_resources/input/lake-small.ppm";
    std::string const expectedOutputPath = "/Users/sanjana/Architecture-Project/test_resources/outputs/maxlevel/lake-small-65535.ppm";

    Image const originalImage = read_ppm(inputPath);
    ImageSOA soa = convertToSOA(originalImage);
    soa.maxlevel(new_max_value65535);

    Image const scaledImage = convertToImage(soa);
    Image const expectedImage = read_ppm(expectedOutputPath);

    ASSERT_TRUE(compareImagesByPixel(scaledImage, expectedImage))
        << "Images differ for maxlevel 65535.";
}
