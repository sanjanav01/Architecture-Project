#include "imagesoa.hpp"
#include "helpers/helpers.hpp" // Include the shared helper file
#include <map>
#include <vector>
#include <algorithm>

// Constructor with width and height parameters
ImageSOA::ImageSOA(const int width, int const height)
    : R(static_cast<size_t>(width * height)), G(static_cast<size_t>(width * height)),
      B(static_cast<size_t>(width * height)), width(width), height(height) {}


// Main cutfreq function, which uses shared helper functions for color analysis
void ImageSOA::cutfreq(int frequency_threshold) {
    // Create a ColorChannels instance to group R, G, and B channels
    ColorChannels channels = {.R=R, .G=G, .B=B};

    auto color_freq = calculateColorFrequencies(channels);
    auto infrequent_colors = getInfrequentColors(color_freq, frequency_threshold);
    replaceInfrequentColors(channels, color_freq, frequency_threshold);
}
#include <cmath>

/*Image ImageSOA::resize(const Image &image, int new_width, int new_height) {
    ImageSOA soa_image(image.width, image.height);
    for (const auto& pixel : image.pixels) {
        soa_image.R.push_back(pixel.r);
        soa_image.G.push_back(pixel.g);
        soa_image.B.push_back(pixel.b);
    }
    Image resized_image(new_width, new_height);
    float const x_scale = static_cast<float>(soa_image.width - 1) / static_cast<float>(new_width - 1);
    float const y_scale = static_cast<float>(soa_image.height - 1) / static_cast<float>(new_height - 1);
    struct InterpolationParams { int c00, c10, c01, c11; float x_frac, y_frac; };
    auto interpolate = [image](const InterpolationParams& params) -> uint16_t {
        float const top = ((1.0F - params.x_frac) * static_cast<float>(params.c00)) + (params.x_frac * static_cast<float>(params.c10));
        float const bottom = ((1.0F - params.x_frac) * static_cast<float>(params.c01)) + (params.x_frac * static_cast<float>(params.c11));
        return static_cast<uint16_t>(std::clamp(std::round(((1.0F - params.y_frac) * top) + (params.y_frac * bottom)), 0.0F, static_cast<float>(image.max_color_value)));
    };
    for (int y_new = 0; y_new < new_height; ++y_new) {
        float const y_pos = static_cast<float>(y_new) * y_scale;
        int const y_floor = static_cast<int>(y_pos);
        int const y_ceil = std::min(y_floor + 1, soa_image.height - 1);
        float const y_frac = y_pos - static_cast<float>(y_floor);
        for (int x_new = 0; x_new < new_width; ++x_new) {
            float const x_pos = static_cast<float>(x_new) * x_scale;
            int const x_floor = static_cast<int>(x_pos);
            int const x_ceil = std::min(x_floor + 1, soa_image.width - 1);
            float const x_frac = x_pos - static_cast<float>(x_floor);
            size_t const idx_yfloor_xfloor = (static_cast<size_t>(y_floor) * static_cast<size_t>(soa_image.width)) + static_cast<size_t>(x_floor);
            size_t const idx_yfloor_xceil = (static_cast<size_t>(y_floor) * static_cast<size_t>(soa_image.width)) + static_cast<size_t>(x_ceil);
            size_t const idx_yceil_xfloor = (static_cast<size_t>(y_ceil) * static_cast<size_t>(soa_image.width)) + static_cast<size_t>(x_floor);
            size_t const idx_yceil_xceil = (static_cast<size_t>(y_ceil) * static_cast<size_t>(soa_image.width)) + static_cast<size_t>(x_ceil);
            InterpolationParams const params_r{.c00=soa_image.R[idx_yfloor_xfloor], .c10=soa_image.R[idx_yfloor_xceil], .c01=soa_image.R[idx_yceil_xfloor], .c11=soa_image.R[idx_yceil_xceil], .x_frac=x_frac, .y_frac=y_frac};
            InterpolationParams const params_g{.c00=soa_image.G[idx_yfloor_xfloor], .c10=soa_image.G[idx_yfloor_xceil], .c01=soa_image.G[idx_yceil_xfloor], .c11=soa_image.G[idx_yceil_xceil], .x_frac=x_frac, .y_frac=y_frac};
            InterpolationParams const params_b{.c00=soa_image.B[idx_yfloor_xfloor], .c10=soa_image.B[idx_yfloor_xceil], .c01=soa_image.B[idx_yceil_xfloor], .c11=soa_image.B[idx_yceil_xceil], .x_frac=x_frac, .y_frac=y_frac};
            Pixel resized_pixel{};
            resized_pixel.r = interpolate(params_r);
            resized_pixel.g = interpolate(params_g);
            resized_pixel.b = interpolate(params_b);
            resized_image.pixels[(static_cast<std::vector<Pixel>::size_type>(y_new) * static_cast<std::vector<Pixel>::size_type>(new_width)) + static_cast<std::vector<Pixel>::size_type>(x_new)] = resized_pixel;}}
    resized_image.max_color_value = image.max_color_value;
    return resized_image;
}*/
/*
ImageSOA ImageSOA::resize_soa(const ImageSOA& input_soa, int new_width, int new_height) {
    ImageSOA resized_soa(new_width, new_height);
    float const x_scale = static_cast<float>(input_soa.width - 1) / static_cast<float>(new_width - 1);
    float const y_scale = static_cast<float>(input_soa.height - 1) / static_cast<float>(new_height - 1);
    auto interpolate = [](int c00, int c10, int c01, int c11, float x_frac, float y_frac) -> int {
        float const top = ((1.0F - x_frac) * static_cast<float>(c00)) + (x_frac * static_cast<float>(c10));
        float const bottom = ((1.0F - x_frac) * static_cast<float>(c01)) + (x_frac * static_cast<float>(c11));
        return static_cast<int>(std::round(((1.0F - y_frac) * top) + (y_frac * bottom)));
    };
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
            resized_soa.R.push_back(interpolate(input_soa.R[idx_yfloor_xfloor], input_soa.R[idx_yfloor_xceil],input_soa.R[idx_yceil_xfloor], input_soa.R[idx_yceil_xceil], x_frac, y_frac));
            resized_soa.G.push_back(interpolate(input_soa.G[idx_yfloor_xfloor], input_soa.G[idx_yfloor_xceil],input_soa.G[idx_yceil_xfloor], input_soa.G[idx_yceil_xceil], x_frac, y_frac));
            resized_soa.B.push_back(interpolate(input_soa.B[idx_yfloor_xfloor], input_soa.B[idx_yfloor_xceil],input_soa.B[idx_yceil_xfloor], input_soa.B[idx_yceil_xceil], x_frac, y_frac));
        }
    }
    resized_soa.width = new_width;
    resized_soa.height = new_height;
    return resized_soa;
}
*/

ImageSOA ImageSOA::resize_soa(const ImageSOA& input_soa, int new_width, int new_height) {
    ImageSOA resized_soa(new_width, new_height);
    float const x_scale = static_cast<float>(input_soa.width - 1) / static_cast<float>(new_width - 1);
    float const y_scale = static_cast<float>(input_soa.height - 1) / static_cast<float>(new_height - 1);
    struct InterpolationParams {
        int c00, c10, c01, c11;
        float x_frac, y_frac;
    };
    auto interpolate = [](const InterpolationParams& params) -> int {
        float const top = ((1.0F - params.x_frac) * static_cast<float>(params.c00)) +(params.x_frac * static_cast<float>(params.c10));
        float const bottom = ((1.0F - params.x_frac) * static_cast<float>(params.c01)) +(params.x_frac * static_cast<float>(params.c11));
        return static_cast<int>(std::round(((1.0F - params.y_frac) * top) + (params.y_frac * bottom)));
    };
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
            resized_soa.R.push_back(interpolate(params_r));
            resized_soa.G.push_back(interpolate(params_g));
            resized_soa.B.push_back(interpolate(params_b));
        }
    }
    resized_soa.width = new_width;
    resized_soa.height = new_height;
    return resized_soa;
}
