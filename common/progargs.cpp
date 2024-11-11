#include "progargs.hpp"
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <array>
#include <ranges>

constexpr int MIN_ARGS = 3;
constexpr int MAXLEVEL_PARAM_INDEX = 4;
constexpr int RESIZE_PARAM_COUNT = 5;
constexpr int CUTFREQ_PARAM_COUNT = 4;
constexpr int MAX_COLOR_VALUE = 65535;

ProgArgs ProgArgs::parse_arguments(int argc, const char* const* argv) {
    // Minimum required arguments check
    if (argc < MIN_ARGS) {
        ProgArgs::display_error("Error: Invalid number of arguments: " + std::to_string(argc), -1);
    }

    ProgArgs parsedArgs;
    parsedArgs.inputFile = argv[1];
    parsedArgs.outputFile = argv[2];
    parsedArgs.operation = argv[3];

    // Collect additional parameters if they exist
    for (int i = 4; i < argc; ++i) {
        parsedArgs.additionalParams.emplace_back(argv[i]);
    }

    // Debugging output to print each additional parameter
    std::cout << "Additional parameters collected: ";
    for (const auto& param : parsedArgs.additionalParams) {
        std::cout << param << " ";
    }
    std::cout << "\n";

    // Validate based on operation type
    if (parsedArgs.operation == "info") {
        if (argc != MIN_ARGS) {
            ProgArgs::display_error("Error: Invalid extra arguments for info.", -1);
        }
    } else if (parsedArgs.operation == "maxlevel") {
        if (argc != MAXLEVEL_PARAM_INDEX) {
            ProgArgs::display_error("Error: Invalid number of extra arguments for maxlevel.", -1);
        }
        int maxLevelValue = std::stoi(parsedArgs.additionalParams[0]);
        if (!isInteger(parsedArgs.additionalParams[0]) || maxLevelValue < 0 || maxLevelValue > MAX_COLOR_VALUE) {
            ProgArgs::display_error("Error: Invalid maxlevel: " + parsedArgs.additionalParams[0], -1);
        }
    } else if (parsedArgs.operation == "resize") {
        if (argc != RESIZE_PARAM_COUNT) {
            ProgArgs::display_error("Error: Invalid number of extra arguments for resize.", -1);
        }
        int resizeWidth = std::stoi(parsedArgs.additionalParams[0]);
        int resizeHeight = std::stoi(parsedArgs.additionalParams[1]);
        if (!isInteger(parsedArgs.additionalParams[0]) || resizeWidth <= 0) {
            ProgArgs::display_error("Error: Invalid resize width: " + parsedArgs.additionalParams[0], -1);
        }
        if (!isInteger(parsedArgs.additionalParams[1]) || resizeHeight <= 0) {
            ProgArgs::display_error("Error: Invalid resize height: " + parsedArgs.additionalParams[1], -1);
        }
    } else if (parsedArgs.operation == "cutfreq") {
        if (argc != CUTFREQ_PARAM_COUNT) {
            ProgArgs::display_error("Error: Invalid number of extra arguments for cutfreq.", -1);
        }
        int cutFreqValue = std::stoi(parsedArgs.additionalParams[0]);
        if (!isInteger(parsedArgs.additionalParams[0]) || cutFreqValue <= 0) {
            ProgArgs::display_error("Error: Invalid cutfreq: " + parsedArgs.additionalParams[0], -1);
        }
    } else if (parsedArgs.operation == "compress") {
        if (argc != MIN_ARGS) {
            ProgArgs::display_error("Error: Invalid extra arguments for compress.", -1);
        }
    } else {
        // If the operation is not recognized, display an error
        ProgArgs::display_error("Error: Invalid option: " + parsedArgs.operation, -1);
    }

    return parsedArgs;
}

[[nodiscard]] std::string ProgArgs::getInputFile() const {
    return inputFile;
}

[[nodiscard]] std::string ProgArgs::getOutputFile() const {
    return outputFile;
}

[[nodiscard]] std::string ProgArgs::getOperation() const {
    return operation;
}

[[nodiscard]] std::vector<std::string> ProgArgs::getAdditionalParams() const {
    return additionalParams;
}

bool ProgArgs::isInteger(const std::string& str) {
    return !str.empty() && std::ranges::all_of(str, ::isdigit);
}

void ProgArgs::display_error(const std::string& error_message, int error_code) {
    std::cerr << error_message << "\n";
    exit(error_code);
}
