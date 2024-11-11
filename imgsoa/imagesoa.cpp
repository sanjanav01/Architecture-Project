#include <vector>
#include <cmath>


ImageSOA resize_soa(const ImageSOA& image, int new_width, int new_height) {
    ImageSOA resized_image;
    resized_image.width = new_width;
    resized_image.height = new_height;
    resized_image.max_value = image.max_value;

    // Resize the color channels in the new image
    resized_image.red_channel.resize(new_width * new_height);
    resized_image.green_channel.resize(new_width * new_height);
    resized_image.blue_channel.resize(new_width * new_height);

    // Scale factors for width and height
    float x_scale = static_cast<float>(image.width) / new_width;
    float y_scale = static_cast<float>(image.height) / new_height;

    // Bilinear interpolation
    for (int y = 0; y < new_height; ++y) {
        for (int x = 0; x < new_width; ++x) {
            // Determine coordinates in the original image
            float src_x = x * x_scale;
            float src_y = y * y_scale;

            int xl = static_cast<int>(std::floor(src_x));
            int xh = std::min(static_cast<int>(std::ceil(src_x)), image.width - 1);
            int yl = static_cast<int>(std::floor(src_y));
            int yh = std::min(static_cast<int>(std::ceil(src_y)), image.height - 1);

            float x_weight = src_x - xl;
            float y_weight = src_y - yl;

            // Interpolate each color channel
            int red_val = static_cast<int>(
                (1 - y_weight) * ((1 - x_weight) * image.red_channel[yl * image.width + xl] +
                                  x_weight * image.red_channel[yl * image.width + xh]) +
                y_weight * ((1 - x_weight) * image.red_channel[yh * image.width + xl] +
                            x_weight * image.red_channel[yh * image.width + xh])
            );

            int green_val = static_cast<int>(
                (1 - y_weight) * ((1 - x_weight) * image.green_channel[yl * image.width + xl] +
                                  x_weight * image.green_channel[yl * image.width + xh]) +
                y_weight * ((1 - x_weight) * image.green_channel[yh * image.width + xl] +
                            x_weight * image.green_channel[yh * image.width + xh])
            );

            int blue_val = static_cast<int>(
                (1 - y_weight) * ((1 - x_weight) * image.blue_channel[yl * image.width + xl] +
                                  x_weight * image.blue_channel[yl * image.width + xh]) +
                y_weight * ((1 - x_weight) * image.blue_channel[yh * image.width + xl] +
                            x_weight * image.blue_channel[yh * image.width + xh])
            );

            // Set pixel in the resized image
            resized_image.red_channel[y * new_width + x] = static_cast<uint16_t>(red_val);
            resized_image.green_channel[y * new_width + x] = static_cast<uint16_t>(green_val);
            resized_image.blue_channel[y * new_width + x] = static_cast<uint16_t>(blue_val);
        }
    }

    return resized_image;
}
