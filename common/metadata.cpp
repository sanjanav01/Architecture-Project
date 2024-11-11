// metadata.cpp
#include "metadata.hpp"

Metadata get_metadata(const Image& image) {
    Metadata metadata;
    metadata.width = image.width;
    metadata.height = image.height;
    metadata.maxColorValue = image.max_color_value;
    return metadata;
}
