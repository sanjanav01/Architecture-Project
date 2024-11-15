#include <gtest/gtest.h>
#include "common/image_types.hpp"
#include "common/binaryio.hpp"
#include "imagesoa.hpp"
#include "helpers.hpp"

#include <chrono> // For execution time measurement
#include <cstdlib> // For std::system

// Constants for test cases
constexpr static int new_max_value100 = 100;
constexpr static int new_max_value1000 = 1000;
constexpr static int new_max_value255 = 255;
constexpr static int new_max_value65535 = 65535;
constexpr static double execution_time = 11.0;

namespace {
  // Convert SOA back to Image for output comparison
  Image convertToImage(const ImageSOA& soa) {
    Image image;
    image.width = soa.width;
    image.height = soa.height;
    image.max_color_value = soa.current_max_color_value; // Use SOA's max color value
    image.pixels.resize(static_cast<size_t>(soa.width) * static_cast<size_t>(soa.height));

    for (size_t i = 0; i < image.pixels.size(); ++i) {
      image.pixels[i].r = static_cast<uint16_t>(
          std::clamp(soa.R[i], 0, static_cast<int>(std::numeric_limits<uint16_t>::max()))
      );
      image.pixels[i].g = static_cast<uint16_t>(
          std::clamp(soa.G[i], 0, static_cast<int>(std::numeric_limits<uint16_t>::max()))
      );
      image.pixels[i].b = static_cast<uint16_t>(
          std::clamp(soa.B[i], 0, static_cast<int>(std::numeric_limits<uint16_t>::max()))
      );
    }
    return image;
  }

  // Convert Image (AOS) to ImageSOA
  ImageSOA convertToSOA(const Image& image) {
    ImageSOA soa(Width{image.width}, Height{image.height}, MaxColorValue{image.max_color_value});
    for (size_t i = 0; i < image.pixels.size(); ++i) {
        soa.R[i] = image.pixels[i].r;
        soa.G[i] = image.pixels[i].g;
        soa.B[i] = image.pixels[i].b;
    }
    return soa;
  }
}

TEST(MaxLevelTestSOA, TestDeerMaxLevel100) {
    std::string const inputPath = "/Users/sanjana/Downloads/input/deer-small.ppm";
    std::string const expectedOutputPath = "/Users/sanjana/Downloads/out/deer-small-100.ppm";
    std::string const generatedOutputPath = "/Users/sanjana/Architecture-Project/test-outputs/deer-small-output-100.ppm";

    Image const originalImage = read_ppm(inputPath);
    ImageSOA soa = convertToSOA(originalImage);
    soa.maxlevel(new_max_value100);

    Image const scaledImage = convertToImage(soa);
    write_ppm(generatedOutputPath, scaledImage);

    ASSERT_TRUE(compareImages(generatedOutputPath, expectedOutputPath))
        << "Images differ for maxlevel 100. Check " << generatedOutputPath;
}

TEST(MaxLevelTestSOA, TestDeerMaxLevel1000) {
    std::string const inputPath = "/Users/sanjana/Downloads/input/deer-small.ppm";
    std::string const expectedOutputPath = "/Users/sanjana/Downloads/out/deer-small-1000.ppm";
    std::string const generatedOutputPath = "/Users/sanjana/Architecture-Project/test-outputs/deer-small-output-1000.ppm";

    Image const originalImage = read_ppm(inputPath);
    ImageSOA soa = convertToSOA(originalImage);
    soa.maxlevel(new_max_value1000);

    Image const scaledImage = convertToImage(soa);
    write_ppm(generatedOutputPath, scaledImage);

    ASSERT_TRUE(compareImages(generatedOutputPath, expectedOutputPath))
        << "Images differ for maxlevel 1000. Check " << generatedOutputPath;
}

TEST(MaxLevelTestSOA, TestDeerMaxLevel255) {
    std::string const inputPath = "/Users/sanjana/Downloads/input/deer-small.ppm";
    std::string const expectedOutputPath = "/Users/sanjana/Downloads/outputs/maxlevel/deer-small-255.ppm";
    std::string const generatedOutputPath = "/Users/sanjana/Architecture-Project/test-outputs/deer-small-output-255.ppm";

    Image const originalImage = read_ppm(inputPath);
    ImageSOA soa = convertToSOA(originalImage);
    soa.maxlevel(new_max_value255);

    Image const scaledImage = convertToImage(soa);
    write_ppm(generatedOutputPath, scaledImage);

    ASSERT_TRUE(compareImages(generatedOutputPath, expectedOutputPath))
        << "Images differ for maxlevel 255. Check " << generatedOutputPath;
}

TEST(MaxLevelTestSOA, TestDeerMaxLevel65535) {
    std::string const inputPath = "/Users/sanjana/Downloads/input/deer-small.ppm";
    std::string const expectedOutputPath = "/Users/sanjana/Downloads/outputs/maxlevel/deer-small-65535.ppm";
    std::string const generatedOutputPath = "/Users/sanjana/Architecture-Project/test-outputs/deer-small-output-65535.ppm";

    Image const originalImage = read_ppm(inputPath);
    ImageSOA soa = convertToSOA(originalImage);
    soa.maxlevel(new_max_value65535);

    Image const scaledImage = convertToImage(soa);
    write_ppm(generatedOutputPath, scaledImage);

    ASSERT_TRUE(compareImages(generatedOutputPath, expectedOutputPath))
        << "Images differ for maxlevel 65535. Check " << generatedOutputPath;
}

TEST(MaxLevelPerformanceTestSOA, LakeLargeMaxLevel65535) {
    std::string const inputPath = "/Users/sanjana/Downloads/input/lake-large.ppm";
    std::string const generatedOutputPath = "/Users/sanjana/Architecture-Project/test-outputs/lake-large-output-65535.ppm";

    Image const originalImage = read_ppm(inputPath);
    ImageSOA soa = convertToSOA(originalImage);

    auto start = std::chrono::high_resolution_clock::now();
    soa.maxlevel(new_max_value65535);
    auto end = std::chrono::high_resolution_clock::now();

    Image const scaledImage = convertToImage(soa);
    write_ppm(generatedOutputPath, scaledImage);

    std::chrono::duration<double> const duration = end - start;
    double const elapsed_time = duration.count();

    ASSERT_LE(elapsed_time, execution_time)
        << "Execution time exceeded for lake-large.ppm with maxlevel 65535";

    std::cout << "Execution time for maxlevel: " << elapsed_time << " seconds\n";
}
