#include <gtest/gtest.h>
#include "common/binaryio.hpp"
#include "imagesoa.hpp"
#include "helpers.hpp"
//constexpr static int FIVE = 5;
//constexpr static int TEN = 10;
constexpr static int HUND = 100;
constexpr static int THOUS = 1000;
constexpr static int EIGHTTHOUS = 8000;
constexpr static double execution_time = 33.0;
namespace {
    Image convertToImage(const ImageSOA& soa) {
        Image image;
        image.width = soa.width;
        image.height = soa.height;
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

TEST(DeerSmall, TestDeerSmallResize1000) {
    std::string const inputPath = "/Users/shriyasingh/Desktop/Architecture-Project/test_resources/input/deer-small.ppm";
    std::string const expectedOutputPath = "/Users/shriyasingh/Desktop/Architecture-Project/test_resources/outputs/resize 2/deer-small-1000.ppm";
    std::string const generatedOutputPath = "/Users/shriyasingh/Desktop/Architecture-Project/test_resources/test_outputs/soa_deersmall_resized1000.ppm";

    Image const inputImage = read_ppm(inputPath);
    ImageSOA const inputSOA = convertToSOA(inputImage);
    Image const expOutput = read_ppm(expectedOutputPath);
    ImageSOA const genOutput = ImageSOA::resize_soa(THOUS, inputSOA, THOUS);
    Image const genImage = convertToImage(genOutput);
    write_ppm(generatedOutputPath, genImage);

    EXPECT_TRUE(compareImagesByPixel(expOutput, genImage)) << "Images differ for resize deer small to 1000";
}

TEST(DeerSmall, TestDeerSmallResize100) {
    std::string const inputPath = "/Users/shriyasingh/Desktop/Architecture-Project/test_resources/input/deer-small.ppm";
    std::string const expectedOutputPath = "/Users/shriyasingh/Desktop/Architecture-Project/test_resources/outputs/resize 2/deer-small-100.ppm";
    std::string const generatedOutputPath = "/Users/shriyasingh/Desktop/Architecture-Project/test_resources/test_outputs/soa_deersmall_resized100.ppm";

    Image const inputImage = read_ppm(inputPath);
    ImageSOA const inputSOA = convertToSOA(inputImage);
    Image const expOutput = read_ppm(expectedOutputPath);
    ImageSOA const genOutput = ImageSOA::resize_soa(HUND, inputSOA, HUND);
    Image const genImage = convertToImage(genOutput);
    write_ppm(generatedOutputPath, genImage);

    EXPECT_TRUE(compareImagesByPixel(expOutput, genImage)) << "Images differ for resize deer small to 100";
}

TEST(DeerLarge, TestDeerLargeResize100) {
    std::string const inputPath = "/Users/shriyasingh/Desktop/Architecture-Project/test_resources/input/deer-large.ppm";
    std::string const expectedOutputPath = "/Users/shriyasingh/Desktop/Architecture-Project/test_resources/outputs/resize 2/deer-large-100.ppm";
    std::string const generatedOutputPath = "/Users/shriyasingh/Desktop/Architecture-Project/test_resources/test_outputs/soa_deerlarge_resized100.ppm";

    Image const inputImage = read_ppm(inputPath);
    ImageSOA const inputSOA = convertToSOA(inputImage);
    Image const expOutput = read_ppm(expectedOutputPath);
    ImageSOA const genOutput = ImageSOA::resize_soa(HUND, inputSOA, HUND);
    Image const genImage = convertToImage(genOutput);
    write_ppm(generatedOutputPath, genImage);

    EXPECT_TRUE(compareImagesByPixel(expOutput, genImage)) << "Images differ for resize deer large to 100";
}

TEST(DeerLarge, TestDeerLargeResize1000) {
    std::string const inputPath = "/Users/shriyasingh/Desktop/Architecture-Project/test_resources/input/deer-large.ppm";
    std::string const expectedOutputPath = "/Users/shriyasingh/Desktop/Architecture-Project/test_resources/outputs/resize 2/deer-large-1000.ppm";
    std::string const generatedOutputPath = "/Users/shriyasingh/Desktop/Architecture-Project/test_resources/test_outputs/soa_deerlarge_resized1000.ppm";

    Image const inputImage = read_ppm(inputPath);
    ImageSOA const inputSOA = convertToSOA(inputImage);
    Image const expOutput = read_ppm(expectedOutputPath);
    ImageSOA const genOutput = ImageSOA::resize_soa(THOUS, inputSOA, THOUS);
    Image const genImage = convertToImage(genOutput);
    write_ppm(generatedOutputPath, genImage);

    EXPECT_TRUE(compareImagesByPixel(expOutput, genImage)) << "Images differ for resize deer large to 1000";
}

TEST(LakeSmall, TestLakeSmallResize1000) {
    std::string const inputPath = "/Users/shriyasingh/Desktop/Architecture-Project/test_resources/input/lake-small.ppm";
    std::string const expectedOutputPath = "/Users/shriyasingh/Desktop/Architecture-Project/test_resources/outputs/resize 2/lake-small-1000.ppm";
    std::string const generatedOutputPath = "/Users/shriyasingh/Desktop/Architecture-Project/test_resources/test_outputs/soa_lakesmall_resized1000.ppm";

    Image const inputImage = read_ppm(inputPath);
    ImageSOA const inputSOA = convertToSOA(inputImage);
    Image const expOutput = read_ppm(expectedOutputPath);
    ImageSOA const genOutput = ImageSOA::resize_soa(THOUS, inputSOA, THOUS);
    Image const genImage = convertToImage(genOutput);
    write_ppm(generatedOutputPath, genImage);

    EXPECT_TRUE(compareImagesByPixel(expOutput, genImage)) << "Images differ for resize lake small to 1000";
}

TEST(LakeSmall, TestLakeSmallResize100) {
    std::string const inputPath = "/Users/shriyasingh/Desktop/Architecture-Project/test_resources/input/lake-small.ppm";
    std::string const expectedOutputPath = "/Users/shriyasingh/Desktop/Architecture-Project/test_resources/outputs/resize 2/lake-small-100.ppm";
    std::string const generatedOutputPath = "/Users/shriyasingh/Desktop/Architecture-Project/test_resources/test_outputs/soa_lakesmall_resized100.ppm";

    Image const inputImage = read_ppm(inputPath);
    ImageSOA const inputSOA = convertToSOA(inputImage);
    Image const expOutput = read_ppm(expectedOutputPath);
    ImageSOA const genOutput = ImageSOA::resize_soa(HUND, inputSOA, HUND);
    Image const genImage = convertToImage(genOutput);
    write_ppm(generatedOutputPath, genImage);

    EXPECT_TRUE(compareImagesByPixel(expOutput, genImage)) << "Images differ for resize lake small to 100";
}

TEST(LakeLarge, TestLakeLargeResize100) {
    std::string const inputPath = "/Users/shriyasingh/Desktop/Architecture-Project/test_resources/input/lake-large.ppm";
    std::string const expectedOutputPath = "/Users/shriyasingh/Desktop/Architecture-Project/test_resources/outputs/resize 2/lake-large-100.ppm";
    std::string const generatedOutputPath = "/Users/shriyasingh/Desktop/Architecture-Project/test_resources/test_outputs/soa_lakelarge_resized100.ppm";

    Image const inputImage = read_ppm(inputPath);
    ImageSOA const inputSOA = convertToSOA(inputImage);
    Image const expOutput = read_ppm(expectedOutputPath);
    ImageSOA const genOutput = ImageSOA::resize_soa(HUND, inputSOA, HUND);
    Image const genImage = convertToImage(genOutput);
    write_ppm(generatedOutputPath, genImage);

    EXPECT_TRUE(compareImagesByPixel(expOutput, genImage)) << "Images differ for resize deer large to 100";
}

TEST(LakeLarge, TestLakeLargeResize1000) {
    std::string const inputPath = "/Users/shriyasingh/Desktop/Architecture-Project/test_resources/input/lake-large.ppm";
    std::string const expectedOutputPath = "/Users/shriyasingh/Desktop/Architecture-Project/test_resources/outputs/resize 2/lake-large-1000.ppm";
    std::string const generatedOutputPath = "/Users/shriyasingh/Desktop/Architecture-Project/test_resources/test_outputs/soa_lakelarge_resized1000.ppm";

    Image const inputImage = read_ppm(inputPath);
    ImageSOA const inputSOA = convertToSOA(inputImage);
    Image const expOutput = read_ppm(expectedOutputPath);
    ImageSOA const genOutput = ImageSOA::resize_soa(THOUS, inputSOA, THOUS);
    Image const genImage = convertToImage(genOutput);
    write_ppm(generatedOutputPath, genImage);

    EXPECT_TRUE(compareImagesByPixel(expOutput, genImage)) << "Images differ for resize deer large to 1000";
}

TEST(LakeSmall, TestLakeSmallResize8000) {
    std::string const inputPath = "/Users/shriyasingh/Desktop/Architecture-Project/test_resources/input/lake-small.ppm";
    std::string const generatedOutputPath = "/Users/shriyasingh/Desktop/Architecture-Project/test_resources/test_outputs/lakesmallsoa_resized8000.ppm";
    std::string const revertedOutputPath = "/Users/shriyasingh/Desktop/Architecture-Project/test_resources/test_outputs/lakesmallsoa_revertedsmall.ppm";

    Image const inputImage = read_ppm(inputPath);
    ImageSOA const inputSOA = convertToSOA(inputImage);
    const auto start = std::chrono::high_resolution_clock::now();
    ImageSOA const genOutput = ImageSOA::resize_soa(EIGHTTHOUS, inputSOA, EIGHTTHOUS);

    Image const genImage = convertToImage(genOutput);
    write_ppm(generatedOutputPath, genImage);
    const auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> const duration = end - start;
    double const elapsed_time = duration.count();
    constexpr double max_allowed_time = execution_time;

    ASSERT_LE(elapsed_time, max_allowed_time) << "Execution time exceeded for resize lake large to 8000 ";
    std::cout << "Execution time for resize: " << elapsed_time << " seconds\n";
    ImageSOA const revertedOutput = ImageSOA::resize_soa(133, genOutput, 100);
    Image const revertedImage = convertToImage(revertedOutput);
    write_ppm(revertedOutputPath, revertedImage);
    EXPECT_TRUE(compareImagesByPixel(revertedImage, inputImage)) << "Images differ for resize lake small to 8000";
}