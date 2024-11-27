#include <iostream>
#include <string>
#include "imgsoa/imagesoa.hpp"
#include "common/binaryio.hpp"
#include "helpers/helpers.hpp"

// Constants for expected argument counts
constexpr static int MAX_ARGUMENTS_INFO = 3;
constexpr static int MAX_ARGUMENTS_MAXLEVEL = 4;
constexpr static int MAX_ARGUMENTS_RESIZE = 5;
constexpr static int MAX_ARGUMENTS_CUTFREQ = 4;
constexpr static int MAX_ARGUMENTS_COMPRESS = 3;

int main(int argc, char* argv[]) {
    try {
        std::vector<std::string> const arguments(argv + 1, argv + argc); // Skip program name
        if (arguments.size() < 3) {std::cerr << "Error: Invalid number of arguments (less than 3): " << (arguments.size()) << '\n'; exit(-1);}
        const std::string& option = arguments[2]; // Extract the option
        auto validate_args = [&](int expected) {
            if (arguments.size() != static_cast<size_t>(expected)) {std::cerr << "Error: Invalid number of arguments for " << option << ": " << arguments.size()<< '\n'; exit(-1);}};
        auto process_soa_operation = [&](auto operation) {
            ImageSOA imageSOA = convertToSOA(read_ppm(arguments[0])); // Convert to SOA
            operation(imageSOA);                                     // Perform the operation
            write_ppm(arguments[1], convertToImage(imageSOA));};       // Write the updated image
        if (option == "info") {
            validate_args(MAX_ARGUMENTS_INFO);
            ImageSOA const imageSOA = convertToSOA(read_ppm(arguments[0]));
            std::cout << "Width: " << imageSOA.width << ", Height: " << imageSOA.height << ", Max Color Value: " << imageSOA.current_max_color_value << '\n';
        } else if (option == "maxlevel") {
            validate_args(MAX_ARGUMENTS_MAXLEVEL);
            int const newMaxValue = std::stoi(arguments[3]);
            if (newMaxValue <= 0) {std::cerr << "Error: Invalid maxlevel value\n";exit(-1);}
            process_soa_operation([&](ImageSOA& imageSOA) { imageSOA.maxlevel(newMaxValue); });
        } else if (option == "resize") {
            validate_args(MAX_ARGUMENTS_RESIZE);
            int const newWidth = std::stoi(arguments[3]);
            int const newHeight = std::stoi(arguments[4]);
            if (newWidth <= 0 || newHeight <= 0) {std::cerr << "Error: Invalid resize dimensions\n";exit(-1);}
            ImageSOA const resizedSOA(Width{newWidth}, Height{newHeight}, MaxColorValue{255});
            write_ppm(arguments[1], convertToImage(resizedSOA));
        } else if (option == "cutfreq") {
            validate_args(MAX_ARGUMENTS_CUTFREQ);
            int const frequencyThreshold = std::stoi(arguments[3]);
            if (frequencyThreshold <= 0) {std::cerr << "Error: Invalid cutfreq value\n";exit(-1);}
            process_soa_operation([&](ImageSOA& imageSOA) { imageSOA.cutfreq(frequencyThreshold); });
        } else if (option == "compress") {
            validate_args(MAX_ARGUMENTS_COMPRESS);
            write_cppm(arguments[1], compress_soa(read_ppm(arguments[0])));
        } else if (option == "decompress") {
            validate_args(MAX_ARGUMENTS_COMPRESS);
            write_ppm(arguments[1], decompress_soa(read_cppm(arguments[0])));
        } else {std::cerr << "Error: Unknown option: " << option << '\n';exit(-1);}
    } catch (const std::exception& e) {std::cerr << "Unhandled exception: " << e.what() << '\n';return -1;}
    return 0;}