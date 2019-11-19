#include "argument.h"

#include <fstream>
#include <iostream>
#include <string>

namespace Init {

bool Argument::Load(int argc, char** argv) {
    if (argc < 2) {
        PrintUsageMessage();
        return false;
    }

    std::string input_file(argv[1]);
    if (!FilenameIsValid(input_file)) {
        PrintErrorMessage(2);
        return false;
    } else if (!FileExists(input_file)) {
        PrintErrorMessage(3);
        return false;
    } else {
        input_file_ = input_file;
    }

    return true;
}

bool Argument::PrintErrorMessage(int error_code) {
    switch (error_code) {
        case 1: std::cerr << "Error 1: No arguments provided.\n"; break;
        case 2: std::cerr << "Error 2: Invalid filename.\n"; break;
        case 3: std::cerr << "Error 3: File does not exist.\n"; break;
        default: std::cerr << "Error: An unknown error occurs.\n"; break;
    }
    PrintUsageMessage();
    return true;
}

bool Argument::PrintUsageMessage() {
    std::cout
        << "usage: sim input_file\n"
        << "  input_file: <file_name>.yo\n"
        << "    Currently only .yo files are accepted.\n"
        << "    The file path should NOT contains spaces or backslashes.\n";
    return true;
}

bool Argument::FilenameIsValid(const std::string& file_name) {
    size_t length = file_name.size();
    if (length <= 3) return false;
    if (file_name.substr(length - 3, 3) != ".yo") return false;
    return true;
}

bool Argument::FileExists(const std::string& file_name) {
    std::ifstream try_file(file_name);
    if (!try_file) return false;
    // Hope the file will not be removed after checking. :-(
    return true;
}

}  // namespace Init
