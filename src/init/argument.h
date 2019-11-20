#ifndef SRC_INIT_ARGUMENT_H_
#define SRC_INIT_ARGUMENT_H_

#include <string>

namespace initialize {

// Collect and handle the arguments passed to the main() function
class Argument {
public:
    bool Load(int argc, char** argv);

    std::string input_file() { return input_file_; }

protected:
    bool PrintInfo();
    bool PrintErrorMessage(int error_code);
    bool PrintUsageMessage();

    bool FilenameIsValid(const std::string& file_name);
    bool FileExists(const std::string& file_name);

    std::string input_file_;
};

}  // namespace initialize

#endif  // SRC_INIT_ARGUMENT_H_
