#ifndef BINARY_IO_HPP
#define BINARY_IO_HPP

#include <string>
#include "image_types.hpp"

Image read_ppm(const std::string& file_path);
void write_ppm(const std::string& file_path, const Image& image);
void write_cppm(const std::string& file_path, const CompressedImage& image);
CompressedImage read_cppm(const std::string& file_path);
#endif
