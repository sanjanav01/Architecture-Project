#include "progargs.hpp"
#include <iostream>
#include <algorithm>

namespace {
    constexpr int MIN_ARGS = 3;
    constexpr int MAXLEVEL_PARAM_INDEX = 4;
    constexpr int RESIZE_PARAM_COUNT = 5;
    constexpr int CUTFREQ_PARAM_COUNT = 4;
    constexpr int MAX_COLOR_VALUE = 65535;
}

ProgArgs ProgArgs::parse_arguments(int argc, const char* const* argv) {
  // Minimum required arguments check
  if (argc < MIN_ARGS) {
    display_error("Error: Invalid number of arguments: " + std::to_string(argc), -1);
  }

  ProgArgs parsedArgs;

  // Use std::next for iterator advancement to avoid pointer arithmetic
  const char* const* arg_iter = std::next(argv, 1); // Start from the first argument after the program name
  parsedArgs.inputFile = *arg_iter;
  parsedArgs.outputFile = *std::next(arg_iter, 1);
  parsedArgs.operation = *std::next(arg_iter, 2);

  // Collect additional parameters if they exist
  for (const auto* param_iter = std::next(arg_iter, 3); param_iter != std::next(argv, argc); param_iter = std::next(param_iter, 1)) {
    parsedArgs.additionalParams.emplace_back(*param_iter);
  }

  if (!validate_operation(parsedArgs, argc)) {
    exit(-1);  // Exit if validation fails
  }

  return parsedArgs;
}




// Helper function to validate the operation type and parameters
bool ProgArgs::validate_operation(const ProgArgs& parsedArgs, int argc) {
    if (parsedArgs.operation == "info") {
        return validate_info(argc);
    }
    if (parsedArgs.operation == "maxlevel") {
        return validate_maxlevel(parsedArgs, argc);
    }
    if (parsedArgs.operation == "resize") {
        return validate_resize(parsedArgs, argc);
    }
    if (parsedArgs.operation == "cutfreq") {
        return validate_cutfreq(parsedArgs, argc);
    }
    if (parsedArgs.operation == "compress") {
        return validate_compress(argc);
    }
    display_error("Error: Invalid option: " + parsedArgs.operation, -1);
    return false;
}

bool ProgArgs::validate_info(int argc) {
    if (argc != MIN_ARGS) {
        display_error("Error: Invalid extra arguments for info.", -1);
        return false;
    }
    return true;
}

bool ProgArgs::validate_maxlevel(const ProgArgs& parsedArgs, int argc) {
    if (argc != MAXLEVEL_PARAM_INDEX) {
        display_error("Error: Invalid number of extra arguments for maxlevel.", -1);
        return false;
    }
    const int maxLevelValue = std::stoi(parsedArgs.additionalParams[0]);
    if (!isInteger(parsedArgs.additionalParams[0]) || maxLevelValue < 0 || maxLevelValue > MAX_COLOR_VALUE) {
        display_error("Error: Invalid maxlevel: " + parsedArgs.additionalParams[0], -1);
        return false;
    }
    return true;
}

bool ProgArgs::validate_resize(const ProgArgs& parsedArgs, int argc) {
    if (argc != RESIZE_PARAM_COUNT) {
        display_error("Error: Invalid number of extra arguments for resize.", -1);
        return false;
    }
    const int resizeWidth = std::stoi(parsedArgs.additionalParams[0]);
    const int resizeHeight = std::stoi(parsedArgs.additionalParams[1]);
    if (!isInteger(parsedArgs.additionalParams[0]) || resizeWidth <= 0) {
        display_error("Error: Invalid resize width: " + parsedArgs.additionalParams[0], -1);
        return false;
    }
    if (!isInteger(parsedArgs.additionalParams[1]) || resizeHeight <= 0) {
        display_error("Error: Invalid resize height: " + parsedArgs.additionalParams[1], -1);
        return false;
    }
    return true;
}

bool ProgArgs::validate_cutfreq(const ProgArgs& parsedArgs, int argc) {
    if (argc != CUTFREQ_PARAM_COUNT) {
        display_error("Error: Invalid number of extra arguments for cutfreq.", -1);
        return false;
    }
    const int cutFreqValue = std::stoi(parsedArgs.additionalParams[0]);
    if (!isInteger(parsedArgs.additionalParams[0]) || cutFreqValue <= 0) {
        display_error("Error: Invalid cutfreq: " + parsedArgs.additionalParams[0], -1);
        return false;
    }
    return true;
}

bool ProgArgs::validate_compress(int argc) {
    if (argc != MIN_ARGS) {
        display_error("Error: Invalid extra arguments for compress.", -1);
        return false;
    }
    return true;
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
