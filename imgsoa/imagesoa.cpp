#include "imagesoa.hpp"
#include <unordered_map>
#include <tuple>

CompressedImageSOA compress_soa(const ImageSOA& image) {
  CompressedImageSOA compressedImage;
  compressedImage.magicNumber = "C6";
  compressedImage.width = image.width;
  compressedImage.height = image.height;
  compressedImage.maxColorValue = image.maxColorValue;

  // Step 1: Create a color table using a hash map to store unique colors
  std::unordered_map<std::tuple<uint16_t, uint16_t, uint16_t>, uint32_t> colorMap;
  std::vector<std::tuple<uint16_t, uint16_t, uint16_t>> colorTable;

  for (size_t i = 0; i < image.red.size(); ++i) {
    auto color = std::make_tuple(image.red[i], image.green[i], image.blue[i]);
    if (!colorMap.contains(color)) { // Use 'contains' instead of 'find'
      colorMap[color] = static_cast<uint32_t>(colorTable.size());
      colorTable.push_back(color);
    }
  }

  compressedImage.colorTableSize = static_cast<int>(colorTable.size()); // Cast to int

  // Step 2: Flatten colorTable into compressedImage.colorTable
  for (const auto& color : colorTable) {
    compressedImage.colorTable.push_back(std::get<0>(color)); // Red
    compressedImage.colorTable.push_back(std::get<1>(color)); // Green
    compressedImage.colorTable.push_back(std::get<2>(color)); // Blue
  }

  // Step 3: Encode each pixel as an index in the color table
  for (size_t i = 0; i < image.red.size(); ++i) {
    auto color = std::make_tuple(image.red[i], image.green[i], image.blue[i]);
    compressedImage.pixelIndices.push_back(colorMap[color]);
  }

  return compressedImage;
}
