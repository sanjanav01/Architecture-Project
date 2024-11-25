#include <iostream>
#include <string>
#include <vector>
#include <span>
#include <cstdlib>
#include "imgaos/imageaos.hpp"
#include "common/binaryio.hpp"
#include "common/metadata.hpp"

constexpr static int MAX_MAXLEVEL = 65535;
constexpr static int FIVE = 5;
constexpr static int SIX = 6;

int main(int const argc, char* argv[]) {
    if (argc < 4) {
    std::cerr << "Error: Insufficient arguments provided.\n";
    return 1;
  }
  std::vector<std::string> args(argv + 1, argv + argc);
    try {
        Image image = read_ppm(args[0]);
        if (args[2] == "info") {
            if (args.size() != 4) { throw std::invalid_argument("Invalid arguments for info");}
            get_metadata(image);
        } else if (args[2] == "maxlevel") {
            if (args.size() != FIVE) { throw std::invalid_argument("Invalid arguments for maxlevel"); }
            int const maxLevel = std::stoi(args[3]);
            if (maxLevel < 0 || maxLevel > MAX_MAXLEVEL) { throw std::out_of_range("maxlevel out of range");}
            ImageAOS::maxlevel(image, maxLevel);
            write_ppm(args[1], image);
        } else if (args[2] == "resize") {
            if (args.size() != SIX) { throw std::invalid_argument("Invalid arguments for resize"); }
            int const newWidth = std::stoi(args[3]);
            int const newHeight = std::stoi(args[4]);
            if (newWidth <= 0 || newHeight <= 0) { throw std::out_of_range("Invalid resize dimensions");}
            const Image resizedImage = ImageAOS::resize(image, newWidth, newHeight);
            write_ppm(args[1], resizedImage);
        } else if (args[2] == "cutfreq") {
            if (args.size() != FIVE) { throw std::invalid_argument("Invalid arguments for cutfreq"); }
            if (int const threshold = std::stoi(args[3]); threshold <= 0) { throw std::out_of_range("Invalid cutfreq threshold"); }
            // image.cutfreq(threshold); // Uncomment when implemented
            write_ppm(args[1], image);
        } else if (args[2] == "compress") {
            if (args.size() != 4) { throw std::invalid_argument("Invalid arguments for compress");}
            // CompressedImage compressedImage = compress_aos(image); // Uncomment when implemented
            // write_cppm(args[1], compressedImage); // Uncomment when implemented
        } else { throw std::invalid_argument("Invalid operation"); }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}