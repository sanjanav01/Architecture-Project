#ifndef PROGARGS_HPP
#define PROGARGS_HPP

#include <string>
#include <vector>

class ProgArgs {
  private:
  ProgArgs() = default;  // Private constructor to enforce static parsing

  std::string inputFile;
  std::string outputFile;
  std::string operation;
  std::vector<std::string> additionalParams;

  static bool isInteger(const std::string& str);  // Utility to validate integers

  public:
  static ProgArgs parse_arguments(int argc, const char* const* argv);  // Factory method to parse arguments

  // Getters for accessing parsed values
  [[nodiscard]] std::string getInputFile() const;
  [[nodiscard]] std::string getOutputFile() const;
  [[nodiscard]] std::string getOperation() const;
  [[nodiscard]] std::vector<std::string> getAdditionalParams() const;

  // Static utility function for error display
  static void display_error(const std::string& error_message, int error_code = -1);
};

#endif  // PROGARGS_HPP
