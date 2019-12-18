#include "argument.h"

#include <fstream>
#include <iostream>
#include <string>

#include "../config.h"

using std::cerr;
using std::cout;
using std::string;

namespace initialize {

bool Argument::Load(int argc, char** argv) {
    if (argc < 2) {
        PrintInfo();
        PrintUsageMessage();
        return false;
    }

    string input_file(argv[1]);
    if (!FilenameIsValid(input_file)) {
        PrintErrorMessage(2);
        return false;
    }
    if (!FileExists(input_file)) {
        PrintErrorMessage(3);
        return false;
    }
    input_file_ = input_file;
    cout << "Successfully read \"" << input_file << "\"\n";

    string output_dir("./");
    if (argc >= 3) output_dir = argv[2];
    output_dir_ = output_dir;
    cout << "Output directory set to \"" << output_dir << "\"\n";

    return true;
}

bool Argument::PrintInfo() {
    cout << "Y86-64 Processor Simulator v";
    cout << kSimVersionMajor << '.' << kSimVersionMinor << '.'
         << kSimVersionPatch << '\n';
    return true;
}

bool Argument::PrintErrorMessage(const int error_code) {
    cerr << "Argument Error ";
    switch (error_code) {
        case 1: cerr << "1: No argument provided.\n"; break;
        case 2: cerr << "2: Invalid filename.\n"; break;
        case 3: cerr << "3: The input file doesn't exist.\n"; break;
        default: cerr << "X: An unknown error occurs.\n"; break;
    }
    PrintUsageMessage();
    return true;
}

bool Argument::PrintUsageMessage() {
    cout << "Usage: ./sim input_file [output_dir]\n"
         << "  input_file: <file_name>.yo\n"
         << "    Currently only .yo files are accepted.\n"
         << "  output_dir\n"
         << "    Optional. Current directory by default.\n";
    return true;
}

bool Argument::FilenameIsValid(const string& file_name) {
    size_t length = file_name.size();
    if (length <= 3) return false;
    if (file_name.substr(length - 3, 3) != ".yo") return false;
    return true;
}

bool Argument::FileExists(const string& file_name) {
    std::ifstream try_file(file_name);
    if (!try_file) return false;
    // Hope the file will not be removed after checking. :-(
    return true;
}

}  // namespace initialize
