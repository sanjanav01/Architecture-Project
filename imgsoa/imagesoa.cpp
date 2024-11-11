#include "imagesoa.hpp"
#include <cmath>

ImageSOA::ImageSOA(int w, int h) : width(w), height(h) {
    // Initialize the R, G, and B channels to the correct size with default values (e.g., 0)
    R.resize(static_cast<size_t>(w * h), 0);
    G.resize(static_cast<size_t>(w * h), 0);
    B.resize(static_cast<size_t>(w * h), 0);
}
ImageSOA ImageSOA::resize_soa(int new_width, int new_height) const {
    ImageSOA resized_image(new_width, new_height);
    float x_scale = static_cast<float>(width) / static_cast<float>(new_width);
    float y_scale = static_cast<float>(height) / static_cast<float>(new_height);
    for (int y = 0; y < new_height; ++y) {
        for (int x = 0; x < new_width; ++x) {
            float src_x = static_cast<float>(x) * x_scale;
            float src_y = static_cast<float>(y) * y_scale;
            int xl = static_cast<int>(std::floor(src_x));
            int xh = std::min(static_cast<int>(std::ceil(src_x)), width - 1);
            int yl = static_cast<int>(std::floor(src_y));
            int yh = std::min(static_cast<int>(std::ceil(src_y)), height - 1);
            float x_weight = src_x - static_cast<float>(xl);
            float y_weight = src_y - static_cast<float>(yl);
            size_t idx_xl_yl = static_cast<size_t>(yl * width + xl);
            size_t idx_xh_yl = static_cast<size_t>(yl * width + xh);
            size_t idx_xl_yh = static_cast<size_t>(yh * width + xl);
            size_t idx_xh_yh = static_cast<size_t>(yh * width + xh);
            int red_val = static_cast<int>((1.0f - y_weight) * ((1.0f - x_weight) * static_cast<float>(R[idx_xl_yl]) + x_weight * static_cast<float>(R[idx_xh_yl])) + y_weight * ((1.0f - x_weight) * static_cast<float>(R[idx_xl_yh]) + x_weight * static_cast<float>(R[idx_xh_yh])));
            int green_val = static_cast<int>((1.0f - y_weight) * ((1.0f - x_weight) * static_cast<float>(G[idx_xl_yl]) + x_weight * static_cast<float>(G[idx_xh_yl])) + y_weight * ((1.0f - x_weight) * static_cast<float>(G[idx_xl_yh]) + x_weight * static_cast<float>(G[idx_xh_yh])));
            int blue_val = static_cast<int>((1.0f - y_weight) * ((1.0f - x_weight) * static_cast<float>(B[idx_xl_yl]) + x_weight * static_cast<float>(B[idx_xh_yl])) + y_weight * ((1.0f - x_weight) * static_cast<float>(B[idx_xl_yh]) + x_weight * static_cast<float>(B[idx_xh_yh])));
            resized_image.R[static_cast<size_t>(y) * static_cast<size_t>(new_width) + static_cast<size_t>(x)] = red_val;
            resized_image.G[static_cast<size_t>(y) * static_cast<size_t>(new_width) + static_cast<size_t>(x)] = green_val;
            resized_image.B[static_cast<size_t>(y) * static_cast<size_t>(new_width) + static_cast<size_t>(x)] = blue_val;
        }
    }
    return resized_image;
}

