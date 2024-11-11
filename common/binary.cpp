#include "binaryio.hpp"
#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

constexpr static int MaxByteValue = 255;
constexpr static int RGB_CHANNELS = 3;
constexpr static int RGB_CHANNELS_16BIT = 6;

Image read_ppm(const std::string& file_path) {
    std::ifstream file(file_path, std::ios::binary);
    if (!file) {throw std::runtime_error("Error: Could not open file " + file_path);}
    std::string magic_number;
    file >> magic_number;
    if (magic_number != "P6") {throw std::runtime_error("Error: Invalid PPM format (not P6)");}
    Image image{};
    file >> image.width >> image.height >> image.max_color_value;
    if (image.width <= 0 || image.height <= 0 || image.max_color_value <= 0) {throw std::runtime_error("Error: Invalid width, height, or max color value in PPM header");}
    file.ignore();
    const size_t total_pixels = static_cast<size_t>(image.width) * static_cast<size_t>(image.height);
    image.pixels.resize(total_pixels);
    if (image.max_color_value < MaxByteValue) {
        for (auto& pixel : image.pixels) {
            std::array<uint8_t, RGB_CHANNELS> rgb{};
            file.read(reinterpret_cast<char*>(rgb.data()), RGB_CHANNELS);
            if (file.gcount() != RGB_CHANNELS) {throw std::runtime_error("Error reading pixel data for 8-bit color with scaling");}
            pixel = {.r=static_cast<uint8_t>(rgb[0] * MaxByteValue / image.max_color_value),.g=static_cast<uint8_t>(rgb[1] * MaxByteValue / image.max_color_value),.b=static_cast<uint8_t>(rgb[2] * MaxByteValue / image.max_color_value)};
        }
    } else if (image.max_color_value == MaxByteValue) {
        for (auto& pixel : image.pixels) {
            std::array<uint8_t, RGB_CHANNELS> rgb{};
            file.read(reinterpret_cast<char*>(rgb.data()), RGB_CHANNELS);
            if (file.gcount() != RGB_CHANNELS) {throw std::runtime_error("Error reading pixel data for 8-bit color without scaling");}
            pixel = {.r=rgb[0], .g=rgb[1], .b=rgb[2]};
        }
    } else {
        for (auto& pixel : image.pixels) {
            std::array<uint8_t, RGB_CHANNELS_16BIT> rgb_bytes{};
            file.read(reinterpret_cast<char*>(rgb_bytes.data()), RGB_CHANNELS_16BIT);
            if (file.gcount() != RGB_CHANNELS_16BIT) {throw std::runtime_error("Error reading pixel data for 16-bit color");}
            const uint16_t red = (static_cast<uint16_t>(rgb_bytes[0]) << 8) | rgb_bytes[1];
            const uint16_t green = (static_cast<uint16_t>(rgb_bytes[2]) << 8) | rgb_bytes[3];
            const uint16_t blue = (static_cast<uint16_t>(rgb_bytes[4]) << 8) | rgb_bytes[5];
            pixel = {.r=static_cast<uint8_t>(red * MaxByteValue / image.max_color_value),.g=static_cast<uint8_t>(green * MaxByteValue / image.max_color_value),.b=static_cast<uint8_t>(blue * MaxByteValue / image.max_color_value)};
        }
    }
    if (file.fail()) {throw std::runtime_error("Error: Unexpected end of file or read error");}
    return image;
}

void write_ppm(const std::string& file_path, const Image& image) {
    std::ofstream out_file(file_path, std::ios::binary);
    if (!out_file) {
        throw std::runtime_error("Could not open file for writing: " + file_path);
    }
    out_file << "P6\n" << image.width << " " << image.height << "\n" << image.max_color_value << "\n";

    bool const use_1_byte_per_channel = (image.max_color_value <= MaxByteValue);
    for (const auto& pixel : image.pixels) {
        if (use_1_byte_per_channel) {
            out_file.put(pixel.r).put(pixel.g).put(pixel.b);
        } else {
            const uint16_t red = pixel.r;
            const uint16_t green = pixel.g;
            const uint16_t blue = pixel.b;
            const std::array<uint8_t, RGB_CHANNELS_16BIT> rgb_bytes = {
                static_cast<uint8_t>(red >> 8), static_cast<uint8_t>(red & 0xFF),
                static_cast<uint8_t>(green >> 8), static_cast<uint8_t>(green & 0xFF),
                static_cast<uint8_t>(blue >> 8), static_cast<uint8_t>(blue & 0xFF)
            };
            out_file.write(reinterpret_cast<const char*>(rgb_bytes.data()), RGB_CHANNELS_16BIT);
        }
    }

    if (!out_file) {
        throw std::runtime_error("Error writing to file: " + file_path);
    }
}

void write_cppm(const std::string& file_path, const CompressedImage& image) {
    std::ofstream file(file_path, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Could not open file for writing: " + file_path);
    }
    file << "C6\n" << image.width << " " << image.height << "\n"
         << image.max_color << "\n" << image.color_table.size() << "\n";

    for (const auto& color : image.color_table) {
        file.write(reinterpret_cast<const char*>(&color), sizeof(uint32_t));
    }

    const size_t index_byte_length = (image.color_table.size() <= 256) ? 1 :
                                     (image.color_table.size() <= 65536) ? 2 : 4;

    for (const auto& index : image.pixel_indices) {
        if (index_byte_length == 1) {
            auto idx = static_cast<uint8_t>(index);
            file.write(reinterpret_cast<const char*>(&idx), 1);
        } else if (index_byte_length == 2) {
            auto idx = static_cast<uint16_t>(index);
            file.write(reinterpret_cast<const char*>(&idx), 2);
        } else {
            auto idx = static_cast<uint32_t>(index);
            file.write(reinterpret_cast<const char*>(&idx), 4);
        }
    }

    if (!file) {
        throw std::runtime_error("Error: Failed to write to file: " + file_path);
    }
    file.close();
}

CompressedImage read_cppm(const std::string& file_path) {
    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open()) {throw std::runtime_error("Error: Could not open file for reading: " + file_path);}
    CompressedImage image{};
    std::string magic;
    file >> magic;
    if (magic != "C6") {throw std::runtime_error("Error: Invalid CPPM format");}
    file >> image.width >> image.height >> image.max_color;
    size_t color_table_size = 0;
    file >> color_table_size;
    file.ignore();
    image.color_table.resize(color_table_size);
    for (auto& color : image.color_table) {file.read(reinterpret_cast<char*>(&color), sizeof(uint32_t));}
    const size_t index_byte_length = (color_table_size <= 256) ? 1 : (color_table_size <= 65536) ? 2 : 4;
    while (file) {
        uint32_t index = 0;
        if (index_byte_length == 1) {
            uint8_t idx = 0;
            file.read(reinterpret_cast<char*>(&idx), 1);
            index = idx;
        } else if (index_byte_length == 2) {
            uint16_t idx = 0;
            file.read(reinterpret_cast<char*>(&idx), 2);
            index = idx;
        } else {
            uint32_t idx = 0;
            file.read(reinterpret_cast<char*>(&idx), 4);
            index = idx;
        }
        if (file) {
            image.pixel_indices.push_back(index);
        }
    }
    file.close();
    return image;
}