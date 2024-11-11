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

  // Utility functions for validation
  static bool isInteger(const std::string& str);
  static bool validate_info(int argc);
  static bool validate_maxlevel(const ProgArgs& parsedArgs, int argc);
  static bool validate_resize(const ProgArgs& parsedArgs, int argc);
  static bool validate_cutfreq(const ProgArgs& parsedArgs, int argc);
  static bool validate_compress(int argc);
  static bool validate_operation(const ProgArgs& parsedArgs, int argc);

  public:
  // Factory method to parse arguments
  static ProgArgs parse_arguments(int argc, const char* const* argv);

  // Getters for accessing parsed values
  [[nodiscard]] std::string getInputFile() const;
  [[nodiscard]] std::string getOutputFile() const;
  [[nodiscard]] std::string getOperation() const;
  [[nodiscard]] std::vector<std::string> getAdditionalParams() const;

  // Static utility function for error display
  static void display_error(const std::string& error_message, int error_code = -1);
};

#endif  // PROGARGS_HPP
