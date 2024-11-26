#include <iostream>
#include <string>
#include "imgaos/imageaos.hpp"
#include "common/binaryio.hpp"
constexpr static int MAX_ARGUMENTS_INFO = 3;
constexpr static int MAX_ARGUMENTS_MAXLEVEL = 4;
constexpr static int MAX_ARGUMENTS_RESIZE = 5;
constexpr static int MAX_ARGUMENTS_CUTFREQ = 4;
constexpr static int MAX_ARGUMENTS_COMPRESS = 3;

int main(int const argc, char* argv[]) {
    try {
        std::vector<std::string> const arguments(argv + 1, argv + argc);
        if (arguments.size() < 3) { std::cerr << "Error: Invalid number of arguments help: " << (arguments.size() - 1) << '\n'; exit(-1);}
        const std::string& option = arguments[2];
        auto validate_args = [&](int const expected) { if (arguments.size() != static_cast<size_t>(expected)) { std::cerr << "Error: Invalid number of arguments for " << option << (arguments.size()) << expected << '\n'; exit(-1); }};
        if (option == "info") {
            validate_args(MAX_ARGUMENTS_INFO);
            Image const image = read_ppm(arguments[0]);
            std::cout << "Width: " << image.width << ", Height: " << image.height << ", Max Color Value: " << image.max_color_value << '\n';
        } else if (option == "maxlevel") {
            validate_args(MAX_ARGUMENTS_MAXLEVEL);
            int const newMaxValue = std::stoi(arguments[3]);
            if (newMaxValue <= 0) { std::cerr << "Error: Invalid maxlevel value\n"; exit(-1);}
            Image image = read_ppm(arguments[0]);
            ImageAOS::maxlevel(image, newMaxValue);
            write_ppm(arguments[1], image);
        } else if (option == "resize") {
            validate_args(MAX_ARGUMENTS_RESIZE);
            int const newWidth = std::stoi(arguments[3]);
            int const newHeight = std::stoi(arguments[4]);
            if (newWidth <= 0 || newHeight <= 0) { std::cerr << "Error: Invalid resize dimensions\n"; exit(-1);}
            Image const image = read_ppm(arguments[0]);
            Image const resizedImage = ImageAOS::resize(image, newWidth, newHeight);
            write_ppm(arguments[1], resizedImage);
        } else if (option == "cutfreq") {
            validate_args(MAX_ARGUMENTS_CUTFREQ);
            int const frequencyThreshold = std::stoi(arguments[3]);
            if (frequencyThreshold <= 0) { std::cerr << "Error: Invalid cutfreq value\n"; exit(-1);}
            Image const image = read_ppm(arguments[0]);
            ImageAOS imageAOS(image.width, image.height);
            imageAOS.cutfreq(frequencyThreshold);
            write_ppm(arguments[1], image);
        } else if (option == "compress") {
            validate_args(MAX_ARGUMENTS_COMPRESS);
            write_cppm(arguments[1], compress_aos(read_ppm(arguments[0])));
        } else if (option == "decompress") {
            validate_args(MAX_ARGUMENTS_COMPRESS);
            write_ppm(arguments[1], decompress(read_cppm(arguments[0])));
        } else { std::cerr << "Error: Unknown option: " << option << '\n'; exit(-1);}
    } catch (const std::exception& e) { std::cerr << "Unhandled exception: " << e.what() << '\n'; exit(-1); } return 0; }
