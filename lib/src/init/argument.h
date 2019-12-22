#ifndef SRC_INIT_ARGUMENT_H_
#define SRC_INIT_ARGUMENT_H_

#include <string>

namespace initialize {

// Collects and handles the arguments passed to the main() function
class Argument {
public:
    bool Load(int argc, char** argv);

    std::string input_file() const { return input_file_; }
    std::string output_dir() const { return output_dir_; }

protected:
    static bool PrintInfo();
    static bool PrintErrorMessage(const int error_code);
    static bool PrintUsageMessage();

    static bool FilenameIsValid(const std::string& file_name);
    static bool FileExists(const std::string& file_name);

    std::string input_file_;
    std::string output_dir_;
};

}  // namespace initialize

#endif  // SRC_INIT_ARGUMENT_H_
