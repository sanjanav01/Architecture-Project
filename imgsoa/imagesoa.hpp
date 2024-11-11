// imagesoa.hpp

#ifndef IMAGESOA_HPP
#define IMAGESOA_HPP

#include <map>
#include <tuple>
#include <vector>

class ImageSOA {
public:
    std::vector<int> R;
    std::vector<int> G;
    std::vector<int> B;
    int width;
    int height;

    // Constructor to initialize the image dimensions
    ImageSOA(int w, int h);

    // Function to remove infrequent colors
    void cutfreq(int frequency_threshold);
    std::map<std::tuple<int, int, int>, int> calculateColorFrequencies() const;
    std::vector<std::tuple<int, int, int>>
        getInfrequentColors(std::map<std::tuple<int, int, int>, int> const & color_freq,
                            int frequency_threshold) const;
    void replaceInfrequentColors(std::map<std::tuple<int, int, int>, int> const & color_freq,
                                 std::vector<std::tuple<int, int, int>> const & infrequent_colors,
                                 int frequency_threshold);
    std::tuple<int, int, int>
        findClosestColor(std::tuple<int, int, int> const & color,
                         std::map<std::tuple<int, int, int>, int> const & color_freq,
                         int frequency_threshold) const;
    // Function to resize the image
    ImageSOA resize_soa(int new_width, int new_height) const;


private:
    // Helper function to find the closest replacement color
    std::tuple<int, int, int> findClosestColor(const std::tuple<int, int, int>& color,
                                               const std::map<std::tuple<int, int, int>, int>& color_freq,
                                               int frequency_threshold);
};

#endif // IMAGESOA_HPP