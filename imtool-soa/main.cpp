#include <iostream>
#include <string>
#include <cstdlib>
#include "imgsoa/imagesoa.hpp"
#include "common/binaryio.hpp"
#include "common/metadata.hpp"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Error: Invalid number of arguments: " << (argc - 1) << '\n';
        return -1;
    }
    std::string const option = argv[3];
    if (option == "info") {
        if (argc != 4) {
            std::cerr << "Error: Invalid number of extra arguments for info: " << (argc - 3) << '\n';
            return -1;
        }
        Image const image = read_ppm(argv[1]);
        get_metadata(image);
    }
    else if (option == "maxlevel") {
        if (argc != 5) {
            std::cerr << "Error: Invalid number of extra arguments for maxlevel: " << (argc - 3) << '\n';
            return -1;
        }
        int const maxLevel = std::atoi(argv[4]);
        if (maxLevel < 0 || maxLevel > 65535) {
            std::cerr << "Error: Invalid maxlevel: " << argv[4] << '\n';
            return -1;
        }
        Image image = read_ppm(argv[1]);
        ImageSOA::maxlevel(maxLevel);
        write_ppm(argv[2], image);
    }
    else if (option == "resize") {
        if (argc != 6) {
            std::cerr << "Error: Invalid number of extra arguments for resize: " << (argc - 3) << '\n';
            return -1;
        }
        int newWidth = std::atoi(argv[4]);
        int newHeight = std::atoi(argv[5]);
        if (newWidth <= 0 || newHeight <= 0) {
            std::cerr << "Error: Invalid resize dimensions: " << argv[4] << " " << argv[5] << '\n';
            return -1;
        }
        Image image = read_ppm(argv[1]);
        Image resizedImage = ImageSOA::resize_soa(newWidth, image, newHeight);
        write_ppm(argv[2], resizedImage);
    }
    else if (option == "cutfreq") {
        if (argc != 5) {
            std::cerr << "Error: Invalid number of extra arguments for cutfreq: " << (argc - 3) << '\n';
            return -1;
        }
        int threshold = std::atoi(argv[4]);
        if (threshold <= 0) {
            std::cerr << "Error: Invalid cutfreq: " << argv[4] << '\n';
            return -1;
        }
        Image image = read_ppm(argv[1]);
        //image.cutfreq(threshold);
        write_ppm(argv[2], image);
    }
    else if (option == "compress") {
        if (argc != 4) {
            std::cerr << "Error: Invalid number of extra arguments for compress: " << (argc - 3) << '\n';
            return -1;
        }
        Image image = read_ppm(argv[1]);
        CompressedImage compressedImage = compress_soa(image);
        write_cppm(argv[2], compressedImage);
    }
    else {
        std::cerr << "Error: Invalid option: " << option << '\n';
        return -1;
    }
    return 0;
}