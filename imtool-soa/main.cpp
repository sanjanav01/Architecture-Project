#include <iostream>
#include <string>
#include <cstdlib>
#include "imgsoa/imagesoa.hpp"
#include "common/binaryio.hpp"
#include "common/metadata.hpp"

namespace {
    // Convert SOA back to Image for output comparison
    Image convertToImage(const ImageSOA& soa) {
        Image image;
        image.width = soa.width;
        image.height = soa.height;
        image.max_color_value = soa.current_max_color_value;
        image.pixels.resize(static_cast<size_t>(soa.width) * static_cast<size_t>(soa.height));

        for (size_t i = 0; i < image.pixels.size(); ++i) {
            image.pixels[i].r = static_cast<uint16_t>(std::clamp(soa.R[i], 0, static_cast<int>(std::numeric_limits<uint16_t>::max())));
            image.pixels[i].g = static_cast<uint16_t>(std::clamp(soa.G[i], 0, static_cast<int>(std::numeric_limits<uint16_t>::max())));
            image.pixels[i].b = static_cast<uint16_t>(std::clamp(soa.B[i], 0, static_cast<int>(std::numeric_limits<uint16_t>::max())));
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
        ImageSOA soaImage = convertToSOA(image); // Convert to SOA
        soaImage.maxlevel(maxLevel); // Call maxlevel on the ImageSOA object
        Image resultImage = convertToImage(soaImage); // Convert back to Image
        write_ppm(argv[2], resultImage);
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
        ImageSOA soaImage = convertToSOA(image); // Convert to SOA
        ImageSOA resizedImage = soaImage.resize_soa(newWidth, soaImage, newHeight); // Resize in SOA with correct parameters
        Image resultImage = convertToImage(resizedImage); // Convert back to Image
        write_ppm(argv[2], resultImage);
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
        ImageSOA soaImage = convertToSOA(image); // Convert to SOA
        soaImage.cutfreq(threshold); // Call cutfreq on the ImageSOA object
        Image resultImage = convertToImage(soaImage); // Convert back to Image
        write_ppm(argv[2], resultImage);
    }
    else if (option == "compress") {
        if (argc != 4) {
            std::cerr << "Error: Invalid number of extra arguments for compress: " << (argc - 3) << '\n';
            return -1;
        }
        Image image = read_ppm(argv[1]);
        //ImageSOA soaImage = convertToSOA(image); // Convert to SOA
        CompressedImage compressedImage = compress_soa(image); // Compress from SOA
        write_cppm(argv[2], compressedImage);
    }
    else {
        std::cerr << "Error: Invalid option: " << option << '\n';
        return -1;
    }

    return 0;
}
