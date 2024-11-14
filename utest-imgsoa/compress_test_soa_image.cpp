#include <gtest/gtest.h>
#include "common/image_types.hpp"
#include "common/binaryio.hpp"
#include "imgsoa/imagesoa.hpp"
#include "helpers/helpers.hpp"
#include <filesystem>
#include <iostream>

TEST(CompressTestSOA, DeerSmall) {
    const std::string inputPath = "/Users/kalyani/Documents/CompArch/Input:Output/input/deer-small.ppm";
    const std::string generatedOutputPath = "/Users/kalyani/Documents/CompArch/Architecture-Project/test-outputs/deer-small-output.cppm";
    const std::string decompressedOutputPath = "/Users/kalyani/Documents/CompArch/Architecture-Project/test-outputs/deer-small-decompressed.ppm";

    // Load the image, compress it using SoA, and save the compressed image
    const Image inputImage = read_ppm(inputPath);
    const CompressedImage compressedImage = compress_soa(inputImage);
    write_cppm(generatedOutputPath, compressedImage);

    // Decompress the compressed image using SoA and save the decompressed output
    const Image decompressedImage = decompress_soa(compressedImage);
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
