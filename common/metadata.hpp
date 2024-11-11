#ifndef METADATA_HPP
#define METADATA_HPP

#include <string>
#include "image_types.hpp"

// Structure to hold image metadata
struct Metadata {
    int width;
    int height;
    int maxColorValue;

    // Method to format metadata for display
    [[nodiscard]] std::string toString() const {
        return "Width: " + std::to_string(width) + "\n" +
               "Height: " + std::to_string(height) + "\n" +
               "Max Color Value: " + std::to_string(maxColorValue);
    }
};

// Function declaration
Metadata get_metadata(const Image& image);

#endif // METADATA_HPP
