#include <gtest/gtest.h>
#include "common/image_types.hpp"
#include "common/binaryio.hpp"
#include "imgaos/imageaos.hpp"
#include "helpers/helpers.hpp"

// Place function inside an anonymous namespace for internal linkage
namespace {
    ImageAOS convertToImageAOS(const Image& img) {
        ImageAOS aosImage;
        aosImage.width = img.width;
        aosImage.height = img.height;
        aosImage.maxColorValue = static_cast<uint16_t>(img.max_color_value); // Cast max color value

        aosImage.pixels.reserve(static_cast<size_t>(img.width) * static_cast<size_t>(img.height));
        for (const auto& pixel : img.pixels) {
            const RGB aosPixel = { .red = pixel.r, .green = pixel.g, .blue = pixel.b };
            aosImage.pixels.push_back(aosPixel);
        }

        return aosImage;
    }

    // Conversion function to convert CompressedImageAOS to CompressedImage
    CompressedImage convertToCompressedImage(const CompressedImageAOS& aosImage) {
        CompressedImage img;
        img.width = aosImage.width;
        img.height = aosImage.height;
        img.max_color = aosImage.maxColorValue;

        // Convert each color from uint16_t to uint32_t
        img.color_table.reserve(aosImage.colorTable.size());
        for (const auto& color : aosImage.colorTable) {
            img.color_table.push_back(static_cast<uint32_t>(color));
        }

        // Assign pixel indices
        img.pixel_indices = aosImage.pixelIndices;

        return img;
    }
}

TEST(CompressTest, DeerSmall) {
    // Define paths to input, expected output, and generated output
    const std::string inputPath = "/Users/kalyani/Documents/CompArch/Input:Output/input/deer-small.ppm";
    const std::string expectedOutputPath = "/Users/kalyani/Documents/CompArch/Input:Output/outputs/compress/deer-small.cppm";
    const std::string generatedOutputPath = "/Users/kalyani/Documents/CompArch/Architecture-Project/test-outputs/deer-small-output.cppm";

    // Load and modify the image, save it to generatedOutputPath
    const Image inputImage = read_ppm(inputPath);
    const ImageAOS aosImage = convertToImageAOS(inputImage);
    const CompressedImageAOS compressedAOSImage = compress_aos(aosImage);
    const CompressedImage compressedImage = convertToCompressedImage(compressedAOSImage);

    write_cppm(generatedOutputPath, compressedImage);  // Write the compressed image

    // Use compareImages with cmp to check if the generated image matches the expected output
    ASSERT_TRUE(compareImages(generatedOutputPath, expectedOutputPath)) << "Images differ";
}

