#ifndef METADATA_HPP
#define METADATA_HPP

#include <string>
#include <iostream> // Include iostream for output
#include "image_types.hpp"

// Structure to hold image metadata
struct Metadata {
  int width;
  int height;
  int maxColorValue;

  [[nodiscard]] std::string toString() const {
    return "Width: " + std::to_string(width) + "\n" +
           "Height: " + std::to_string(height) + "\n" +
           "Max Color Value: " + std::to_string(maxColorValue);
  }

  void display() const {
    std::cout << toString() << '\n';
  }
};

Metadata get_metadata(const Image& image);

#endif