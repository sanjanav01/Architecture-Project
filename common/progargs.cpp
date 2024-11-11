#include "progargs.hpp"
#include <iostream>
#include <stdexcept>
#include <algorithm> // for std::find

// Static function to parse command-line arguments and return a ProgArgs object
ProgArgs ProgArgs::parse_arguments(int argc, char* argv[]) {
    if (argc < 3) {
        ProgArgs::display_error("Error: Invalid number of arguments: " + std::to_string(argc), -1);
    }

    ProgArgs parsedArgs;
    parsedArgs.inputFile = argv[1];
    parsedArgs.outputFile = argv[2];
    parsedArgs.operation = argv[3];

    // Collect additional parameters for maxlevel, resize, cutfreq
    for (int i = 4; i < argc; ++i) {
        parsedArgs.additionalParams.push_back(argv[i]);
    }

    // Argument validation logic based on the operation type
    if (parsedArgs.operation == "info" && argc != 3) {
        ProgArgs::display_error("Error: Invalid extra arguments for info.", -1);
    } else if (parsedArgs.operation == "maxlevel") {
        if (argc != 4) {
            ProgArgs::display_error("Error: Invalid number of extra arguments for maxlevel: " + std::to_string(argc - 3), -1);
        }
        if (!isInteger(parsedArgs.additionalParams[0]) || std::stoi(parsedArgs.additionalParams[0]) < 0 || std::stoi(parsedArgs.additionalParams[0]) > 65535) {
            ProgArgs::display_error("Error: Invalid maxlevel: " + parsedArgs.additionalParams[0], -1);
        }
    } else if (parsedArgs.operation == "resize") {
        if (argc != 5) {
            ProgArgs::display_error("Error: Invalid number of extra arguments for resize: " + std::to_string(argc - 3), -1);
        }
        if (!isInteger(parsedArgs.additionalParams[0]) || std::stoi(parsedArgs.additionalParams[0]) <= 0) {
            ProgArgs::display_error("Error: Invalid resize width: " + parsedArgs.additionalParams[0], -1);
        }
        if (!isInteger(parsedArgs.additionalParams[1]) || std::stoi(parsedArgs.additionalParams[1]) <= 0) {
            ProgArgs::display_error("Error: Invalid resize height: " + parsedArgs.additionalParams[1], -1);
        }
    } else if (parsedArgs.operation == "cutfreq") {
        if (argc != 4) {
            ProgArgs::display_error("Error: Invalid number of extra arguments for cutfreq: " + std::to_string(argc - 3), -1);
        }
        if (!isInteger(parsedArgs.additionalParams[0]) || std::stoi(parsedArgs.additionalParams[0]) <= 0) {
            ProgArgs::display_error("Error: Invalid cutfreq: " + parsedArgs.additionalParams[0], -1);
        }
    } else if (parsedArgs.operation == "compress" && argc != 3) {
        ProgArgs::display_error("Error: Invalid extra arguments for compress.", -1);
    } else if (parsedArgs.operation != "info" && parsedArgs.operation != "maxlevel" &&
               parsedArgs.operation != "resize" && parsedArgs.operation != "cutfreq" &&
               parsedArgs.operation != "compress") {
        ProgArgs::display_error("Error: Invalid option: " + parsedArgs.operation, -1);
    }

    return parsedArgs;
}

// Getter for input file path
std::string ProgArgs::getInputFile() const {
    return inputFile;
}

// Getter for output file path
std::string ProgArgs::getOutputFile() const {
    return outputFile;
}

// Getter for operation
std::string ProgArgs::getOperation() const {
    return operation;
}

// Getter for additional parameters
std::vector<std::string> ProgArgs::getAdditionalParams() const {
    return additionalParams;
}

bool ProgArgs::isInteger(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

// Definition for display_error function
void ProgArgs::display_error(const std::string& error_message, int error_code) {
    std::cerr << error_message << std::endl;
    exit(error_code);
}