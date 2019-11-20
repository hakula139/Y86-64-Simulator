#ifndef SRC_INIT_ARGUMENT_H_
#define SRC_INIT_ARGUMENT_H_

#include <string>

namespace initialize {

// Collect and handle the arguments passed to the main() function
class Argument {
public:
    bool Load(int argc, char** argv);

    std::string input_file() const { return input_file_; }

protected:
    bool PrintInfo() const;
    bool PrintErrorMessage(int error_code) const;
    bool PrintUsageMessage() const;

    bool FilenameIsValid(const std::string& file_name) const;
    bool FileExists(const std::string& file_name) const;

    std::string input_file_;
};

}  // namespace initialize

#endif  // SRC_INIT_ARGUMENT_H_
