#include <iostream>
#include <string>
#include <cstdlib>
#include <span>
#include <vector>
#include "imgaos/imageaos.hpp"
#include "common/binaryio.hpp"
#include "common/metadata.hpp"

constexpr static int MAX_ARGUMENTS_INFO = 4;
constexpr static int MAX_ARGUMENTS_MAXLEVEL = 5;
constexpr static int MAX_ARGUMENTS_RESIZE = 6;
constexpr static int MAX_ARGUMENTS_CUTFREQ = 5;
constexpr static int MAX_ARGUMENTS_COMPRESS = 4;
constexpr static int MAX_MAXLEVEL = 65535;

namespace {

// Helper function to handle "info" option
void handle_info(const std::vector<std::string>& arguments) {
    if (arguments.size() != MAX_ARGUMENTS_INFO) {
        std::cerr << "Error: Invalid number of extra arguments for info: " << (arguments.size() - 3) << '\n';
        return;
    }
    const Image image = read_ppm(arguments[0]); // 'image' declared const
    get_metadata(image);
}

// Helper function to handle "maxlevel" option
void handle_maxlevel(const std::vector<std::string>& arguments) {
    if (arguments.size() != MAX_ARGUMENTS_MAXLEVEL) {
        std::cerr << "Error: Invalid number of extra arguments for maxlevel: " << (arguments.size() - 3) << '\n';
        return;
    }
    char* endptr = nullptr;
    long const maxLevelLong = std::strtol(arguments[3].c_str(), &endptr, 10); // Use strtol
    if (*endptr != '\0' || maxLevelLong < 0 || maxLevelLong > MAX_MAXLEVEL) {
        std::cerr << "Error: Invalid maxlevel: " << arguments[3] << '\n';
        return;
    }
    const int maxLevel = static_cast<int>(maxLevelLong); // Safe cast after validation
    Image image = read_ppm(arguments[0]);
    ImageAOS::maxlevel(image, maxLevel);
    write_ppm(arguments[1], image);
}

// Helper function to handle "resize" option
void handle_resize(const std::vector<std::string>& arguments) {
    if (arguments.size() != MAX_ARGUMENTS_RESIZE) {
        std::cerr << "Error: Invalid number of extra arguments for resize: " << (arguments.size() - 3) << '\n';
        return;
    }
    char* endptr_width = nullptr;
    char* endptr_height = nullptr;
    long const newWidthLong = std::strtol(arguments[3].c_str(), &endptr_width, 10); // Use strtol
    long const newHeightLong = std::strtol(arguments[4].c_str(), &endptr_height, 10); // Use strtol
    if (*endptr_width != '\0' || *endptr_height != '\0' || newWidthLong <= 0 || newHeightLong <= 0) {
        std::cerr << "Error: Invalid resize dimensions: " << arguments[3] << " " << arguments[4] << '\n';
        return;
    }
    const int newWidth = static_cast<int>(newWidthLong); // Safe cast after validation
    const int newHeight = static_cast<int>(newHeightLong); // Safe cast after validation
    const Image image = read_ppm(arguments[0]); // 'image' declared const
    const Image resizedImage = ImageAOS::resize(image, newWidth, newHeight); // 'resizedImage' declared const
    write_ppm(arguments[1], resizedImage);
}

// Helper function to handle "cutfreq" option
void handle_cutfreq(const std::vector<std::string>& arguments) {
    if (arguments.size() != MAX_ARGUMENTS_CUTFREQ) {
        std::cerr << "Error: Invalid number of extra arguments for cutfreq: " << (arguments.size() - 3) << '\n';
        return;
    }
    char* endptr = nullptr;
    long const thresholdLong = std::strtol(arguments[3].c_str(), &endptr, 10); // Use strtol
    if (*endptr != '\0' || thresholdLong <= 0) {
        std::cerr << "Error: Invalid cutfreq: " << arguments[3] << '\n';
        return;
    }
    Image const image = read_ppm(arguments[0]); // 'image' declared const
    // image.cutfreq(threshold); // Uncomment when implemented
    write_ppm(arguments[1], image);
}

// Helper function to handle "compress" option
void handle_compress(const std::vector<std::string>& arguments) {
    if (arguments.size() != MAX_ARGUMENTS_COMPRESS) {
        std::cerr << "Error: Invalid number of extra arguments for compress: " << (arguments.size() - 3) << '\n';
        return;
    }
    const Image image = read_ppm(arguments[0]); // 'image' declared const
    // CompressedImage compressedImage = CompressedImageAOS::compress(image); // Uncomment when implemented
    // compress_aos(image); // Uncomment when implemented
    // write_cppm(arguments[1], compressedImage); // Uncomment when implemented
}

// Main process_arguments function
void process_arguments(const std::vector<std::string>& arguments) {
    if (arguments.size() < 3) {
        std::cerr << "Error: Invalid number of arguments: " << (arguments.size() - 1) << '\n';
        return;
    }

    const std::string& option = arguments[2];
    if (option == "info") {
        handle_info(arguments);
    } else if (option == "maxlevel") {
        handle_maxlevel(arguments);
    } else if (option == "resize") {
        handle_resize(arguments);
    } else if (option == "cutfreq") {
        handle_cutfreq(arguments);
    } else if (option == "compress") {
        handle_compress(arguments);
    } else {
        std::cerr << "Error: Invalid option: " << option << '\n';
    }
}
} // namespace

int main(int argc, char* argv[]) {
    // Create a span over argv and convert to a vector of strings for safer handling
    std::span<char*> const args_view(argv, static_cast<size_t>(argc));
    std::vector<std::string> const arguments(args_view.begin() + 1, args_view.end()); // Skip program name
    process_arguments(arguments);
    return 0;
}