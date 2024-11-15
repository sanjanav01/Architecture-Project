#include "imagesoa.hpp"
#include "iostream"
#include <vector>
#include <algorithm>
#include <cmath>
constexpr static int MAX_VAL = 256;

ImageSOA::ImageSOA(Width const width, Height const height, MaxColorValue const max_color_value)
    : width(width.value),
      height(height.value),
      current_max_color_value(max_color_value.value),
      R(static_cast<size_t>(width.value * height.value)),
      G(static_cast<size_t>(width.value * height.value)),
      B(static_cast<size_t>(width.value * height.value))
{}

ImageSOA::ImageSOA(Width const width, Height const height)
    : width(width.value),
      height(height.value),
      current_max_color_value(MAX_VAL),
        R(static_cast<size_t>(width.value * height.value)),
        G(static_cast<size_t>(width.value * height.value)),
        B(static_cast<size_t>(width.value * height.value))
{}

/*
namespace {
    void printVector(const std::vector<int>& vec, const std::string& label) {
        std::cout << label << ": [";
        for (size_t i = 0; i < vec.size(); ++i) {
            std::cout << vec[i];
            if (i < vec.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "]" << '\n';
    } */
struct InterpolationParams {
    int c00, c10, c01, c11;
    float x_frac, y_frac;
};

ImageSOA ImageSOA::resize_soa(const int new_width, const ImageSOA& input_soa, int new_height) {
    Width const width = {new_width};
    Height const height = {new_height};
    ImageSOA resized_soa(width, height);
    resized_soa.R.clear();
    resized_soa.G.clear();
    resized_soa.B.clear();
    float const x_scale = static_cast<float>(input_soa.width - 1) / static_cast<float>(new_width - 1);
    float const y_scale = static_cast<float>(input_soa.height - 1) / static_cast<float>(new_height - 1);
    auto interpolate = [](const InterpolationParams& params) -> int {
        float const top = ((1.0F - params.x_frac) * static_cast<float>(params.c00)) +(params.x_frac * static_cast<float>(params.c10));
        float const bottom = ((1.0F - params.x_frac) * static_cast<float>(params.c01)) +(params.x_frac * static_cast<float>(params.c11));
        return static_cast<int>(std::round(((1.0F - params.y_frac) * top) + (params.y_frac * bottom)));};
    for (int y_new = 0; y_new < new_height; ++y_new) {
        float const y_pos = static_cast<float>(y_new) * y_scale;
        int const y_floor = static_cast<int>(y_pos);
        int const y_ceil = std::min(y_floor + 1, input_soa.height - 1);
        float const y_frac = y_pos - static_cast<float>(y_floor);
        for (int x_new = 0; x_new < new_width; ++x_new) {
            float const x_pos = static_cast<float>(x_new) * x_scale;
            int const x_floor = static_cast<int>(x_pos);
            int const x_ceil = std::min(x_floor + 1, input_soa.width - 1);
            float const x_frac = x_pos - static_cast<float>(x_floor);
            size_t const idx_yfloor_xfloor = (static_cast<size_t>(y_floor) * static_cast<size_t>(input_soa.width)) + static_cast<size_t>(x_floor);
            size_t const idx_yfloor_xceil = (static_cast<size_t>(y_floor) * static_cast<size_t>(input_soa.width)) + static_cast<size_t>(x_ceil);
            size_t const idx_yceil_xfloor = (static_cast<size_t>(y_ceil) * static_cast<size_t>(input_soa.width)) + static_cast<size_t>(x_floor);
            size_t const idx_yceil_xceil = (static_cast<size_t>(y_ceil) * static_cast<size_t>(input_soa.width)) + static_cast<size_t>(x_ceil);
            InterpolationParams const params_r{.c00 = input_soa.R[idx_yfloor_xfloor],.c10 = input_soa.R[idx_yfloor_xceil],.c01 = input_soa.R[idx_yceil_xfloor],.c11 = input_soa.R[idx_yceil_xceil],.x_frac = x_frac,.y_frac = y_frac};
            InterpolationParams const params_g{.c00 = input_soa.G[idx_yfloor_xfloor],.c10 = input_soa.G[idx_yfloor_xceil],.c01 = input_soa.G[idx_yceil_xfloor],.c11 = input_soa.G[idx_yceil_xceil],.x_frac = x_frac,.y_frac = y_frac};
            InterpolationParams const params_b{.c00 = input_soa.B[idx_yfloor_xfloor],.c10 = input_soa.B[idx_yfloor_xceil],.c01 = input_soa.B[idx_yceil_xfloor],.c11 = input_soa.B[idx_yceil_xceil],.x_frac = x_frac,.y_frac = y_frac};
            int const interpolated_r = interpolate(params_r);
            int const interpolated_g = interpolate(params_g);
            int const interpolated_b = interpolate(params_b);
            resized_soa.R.emplace_back(interpolated_r);
            resized_soa.G.emplace_back(interpolated_g);
            resized_soa.B.emplace_back(interpolated_b);
        }
    }
    resized_soa.width = new_width;resized_soa.height = new_height;
    return resized_soa;
}
