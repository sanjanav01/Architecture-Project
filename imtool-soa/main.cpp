#include <iostream>
#include <string>
#include <cstdlib>
#include "imgsoa/imagesoa.hpp"
#include "common/binaryio.hpp"
#include "common/metadata.hpp"

// Constants for expected argument counts
constexpr static int MAX_ARGUMENTS_INFO = 4;
constexpr static int MAX_ARGUMENTS_MAXLEVEL = 5;
constexpr static int MAX_ARGUMENTS_RESIZE = 6;
constexpr static int MAX_ARGUMENTS_CUTFREQ = 5;
constexpr static int MAX_ARGUMENTS_COMPRESS = 4;

namespace {
  void process_arguments(const std::vector<std::string>& arguments) {
    if (arguments.size() < 3) {
        std::cerr << "Error: Invalid number of arguments: " << (arguments.size() - 1) << '\n';
        return;
    }

    const std::string& option = arguments[2]; // Extract the option

    if (option == "info") {
        if (arguments.size() != MAX_ARGUMENTS_INFO) {
            std::cerr << "Error: Invalid number of arguments for info\n";
            return;
        }
        Image originalImage = read_ppm(arguments[0]);
        ImageSOA imageSOA = convertToSOA(originalImage); // Convert to SOA
        std::cout << "Width: " << imageSOA.width << ", Height: " << imageSOA.height
                  << ", Max Color Value: " << imageSOA.current_max_color_value << '\n';
    } else if (option == "maxlevel") {
        if (arguments.size() != MAX_ARGUMENTS_MAXLEVEL) {
            std::cerr << "Error: Invalid number of arguments for maxlevel\n";
            return;
        }
        int newMaxValue = std::stoi(arguments[3]);
        if (newMaxValue <= 0) {
            std::cerr << "Error: Invalid maxlevel value\n";
            return;
        }

        Image originalImage = read_ppm(arguments[0]);
        ImageSOA imageSOA = convertToSOA(originalImage); // Convert to SOA
        imageSOA.maxlevel(newMaxValue);

        Image updatedImage = convertToImage(imageSOA); // Convert back to AoS
        write_ppm(arguments[1], updatedImage);
    } else if (option == "resize") {
        if (arguments.size() != MAX_ARGUMENTS_RESIZE) {
            std::cerr << "Error: Invalid number of arguments for resize\n";
            return;
        }
        int newWidth = std::stoi(arguments[3]);
        int newHeight = std::stoi(arguments[4]);
        if (newWidth <= 0 || newHeight <= 0) {
            std::cerr << "Error: Invalid resize dimensions\n";
            return;
        }

        Image originalImage = read_ppm(arguments[0]);
        ImageSOA imageSOA = convertToSOA(originalImage); // Convert to SOA
        ImageSOA resizedSOA(Width{newWidth}, Height{newHeight}, MaxColorValue{imageSOA.current_max_color_value});

        // Resize logic should be implemented separately in ImageSOA
        Image updatedImage = convertToImage(resizedSOA); // Convert back to AoS
        write_ppm(arguments[1], updatedImage);
    } else if (option == "cutfreq") {
        if (arguments.size() != MAX_ARGUMENTS_CUTFREQ) {
            std::cerr << "Error: Invalid number of arguments for cutfreq\n";
            return;
        }
        int frequencyThreshold = std::stoi(arguments[3]);
        if (frequencyThreshold <= 0) {
            std::cerr << "Error: Invalid cutfreq value\n";
            return;
        }

        Image originalImage = read_ppm(arguments[0]);
        ImageSOA imageSOA = convertToSOA(originalImage); // Convert to SOA
        imageSOA.cutfreq(frequencyThreshold);

        Image updatedImage = convertToImage(imageSOA); // Convert back to AoS
        write_ppm(arguments[1], updatedImage);
    } else if (option == "compress") {
        if (arguments.size() != MAX_ARGUMENTS_COMPRESS) {
            std::cerr << "Error: Invalid number of arguments for compress\n";
            return;
        }

        Image originalImage = read_ppm(arguments[0]);
        CompressedImage compressedImage = compress_soa(originalImage);
        write_cppm(arguments[1], compressedImage);
    } else if (option == "decompress") {
        if (arguments.size() != MAX_ARGUMENTS_COMPRESS) {
            std::cerr << "Error: Invalid number of arguments for decompress\n";
            return;
        }

        CompressedImage compressedImage = read_cppm(arguments[0]);
        Image decompressedImage = decompress_soa(compressedImage);
        write_ppm(arguments[1], decompressedImage);
    } else {
        std::cerr << "Error: Unknown option: " << option << '\n';
    }
}
}


int main(int argc, char* argv[]) {
    std::vector<std::string> arguments(argv + 1, argv + argc); // Skip program name
    process_arguments(arguments);
    return 0;
}
