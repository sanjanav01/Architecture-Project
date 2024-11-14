#include <gtest/gtest.h>
#include "common/image_types.hpp"
#include "common/binaryio.hpp"
#include "imgaos/imageaos.hpp"
#include "helpers/helpers.hpp"
#include <filesystem>
#include <iostream>

// TEST(CompressTest, DeerSmall) {
//     // Define paths to input, expected output, and generated output
//     const std::string inputPath = "/Users/kalyani/Documents/CompArch/Input:Output/input/deer-small.ppm";
//     const std::string expectedOutputPath = "/Users/kalyani/Documents/CompArch/Input:Output/outputs/compress/deer-small.cppm";
//     const std::string generatedOutputPath = "/Users/kalyani/Documents/CompArch/Architecture-Project/test-outputs/deer-small-output.cppm";
//
//     // Load the image, compress it, and save the compressed image
//     const Image inputImage = read_ppm(inputPath);
//     const CompressedImage compressedImage = compress_aos(inputImage); // Use compress_aos with Image input
//     write_cppm(generatedOutputPath, compressedImage);  // Write the compressed image
//
//     // Check file sizes
//     try {
//         auto inputSize = std::filesystem::file_size(inputPath);
//         auto generatedSize = std::filesystem::file_size(generatedOutputPath);
//         auto expectedSize = std::filesystem::file_size(expectedOutputPath);
//
//         std::cout << "Input file size: " << inputSize << " bytes\n";
//         std::cout << "Generated file size: " << generatedSize << " bytes\n";
//         std::cout << "Expected file size: " << expectedSize << " bytes\n";
//
//         ASSERT_EQ(generatedSize, expectedSize) << "File sizes differ!";
//     } catch (const std::filesystem::filesystem_error& e) {
//         std::cerr << "Error checking file sizes: " << e.what() << '\n';
//         FAIL() << "File size check failed.";
//     }
//
//     // Use compareImages to check if the generated image matches the expected output
//     ASSERT_TRUE(compareImages(generatedOutputPath, expectedOutputPath)) << "Images differ";
// }

// TEST(CompressTest, DeerLarge) {
//   const std::string inputPath = "/Users/kalyani/Documents/CompArch/Input:Output/input/deer-large.ppm";
//   const std::string generatedOutputPath = "/Users/kalyani/Documents/CompArch/Architecture-Project/test-outputs/deer-large-output.cppm";
//   const std::string decompressedOutputPath = "/Users/kalyani/Documents/CompArch/Architecture-Project/test-outputs/deer-large-decompressed.ppm";
//
//   // Load, compress, and write the large image
//   const Image inputImage = read_ppm(inputPath);
//   const CompressedImage compressedImage = compress_aos(inputImage);
//   write_cppm(generatedOutputPath, compressedImage);
//
//   // Decompress and write the output
//   const Image decompressedImage = decompress(compressedImage);
//   write_ppm(decompressedOutputPath, decompressedImage);
//
//   // Metadata checks
//   ASSERT_EQ(inputImage.width, decompressedImage.width);
//   ASSERT_EQ(inputImage.height, decompressedImage.height);
//   ASSERT_EQ(inputImage.max_color_value, decompressedImage.max_color_value);

  // Optional: Binary file-level comparison if metadata matches
  // ASSERT_TRUE(compareImages(decompressedOutputPath, inputPath)) << "Images differ after decompression in binary comparison";
//}

TEST(CompressTest, DeerSmall) {
    const std::string inputPath = "/Users/kalyani/Documents/CompArch/Input:Output/input/deer-small.ppm";
    const std::string generatedOutputPath = "/Users/kalyani/Documents/CompArch/Architecture-Project/test-outputs/deer-small-output.cppm";
    const std::string decompressedOutputPath = "/Users/kalyani/Documents/CompArch/Architecture-Project/test-outputs/deer-small-decompressed.ppm";

    // Load the image, compress it, and save the compressed image
    const Image inputImage = read_ppm(inputPath);
    const CompressedImage compressedImage = compress_aos(inputImage);
    write_cppm(generatedOutputPath, compressedImage);

    // Decompress the compressed image and save the decompressed output
    const Image decompressedImage = decompress(compressedImage);
    write_ppm(decompressedOutputPath, decompressedImage);

    // Basic metadata checks
    ASSERT_EQ(inputImage.width, decompressedImage.width);
    ASSERT_EQ(inputImage.height, decompressedImage.height);
    ASSERT_EQ(inputImage.max_color_value, decompressedImage.max_color_value);
    ASSERT_EQ(inputImage.pixels.size(), decompressedImage.pixels.size());

    // Check each pixel for exact equality
    bool pixels_match = true;
    for (size_t i = 0; i < inputImage.pixels.size(); ++i) {
        if (inputImage.pixels[i].r != decompressedImage.pixels[i].r ||
            inputImage.pixels[i].g != decompressedImage.pixels[i].g ||
            inputImage.pixels[i].b != decompressedImage.pixels[i].b) {
            std::cout << "Pixel mismatch at index " << i
                      << ": Original(R,G,B) = (" << inputImage.pixels[i].r << ", "
                      << inputImage.pixels[i].g << ", " << inputImage.pixels[i].b << ") "
                      << "Decompressed(R,G,B) = (" << decompressedImage.pixels[i].r << ", "
                      << decompressedImage.pixels[i].g << ", " << decompressedImage.pixels[i].b << ")\n";
            pixels_match = false;
            break;  // Stop at first mismatch for debugging clarity
        }
    }
    ASSERT_TRUE(pixels_match) << "Pixels do not match between original and decompressed images.";

    // Now use compareImages on the files, if the pixels match exactly
    if (pixels_match) {
        ASSERT_TRUE(compareImages(decompressedOutputPath, inputPath)) << "Images differ after decompression in binary comparison";
    }
}




