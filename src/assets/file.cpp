#include "file.h"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <utility>

namespace assets {

bool File::ReadRawData() {
    std::ifstream input;
    input.open(input_file_, std::ifstream::in);
    if (!input) {
        PrintErrorMessage(1);
        return false;
    }
    for (std::string line; !input.eof();) {
        std::getline(input, line);
        raw_data_.push_back(line);
    }
    input.close();
    return true;
}

bool File::ReadInstructions() {
    // TODO(Hakula): Cannot work. Need fix.
    uint64_t address = 0;
    for (const auto& line : raw_data_) {
        if (line.empty() || line.front() != '0') continue;
        std::vector<uint8_t> instruction;
        auto                 begin = line.find(':') + 2;
        auto                 i     = begin;
        for (; line[i] != ' '; i += 2) {
            auto code_str = line.substr(i, 2);
            // TODO(Hakula): Check if 'code_str' is a valid hexadecimal number.
            auto code = static_cast<uint8_t>(std::stoi(code_str, nullptr, 16));
            instruction.push_back(code);
        }
        if (instruction.empty()) continue;  // no instruction extracted
        auto address_str = line.substr(0, begin - 2);
        auto address_given =
            static_cast<uint64_t>(std::stoi(address_str, nullptr, 16));
        if (address != address_given) {
            PrintErrorMessage(2);
            return false;
        }
        instructions_.insert(std::make_pair(address, instruction));
        address += i - begin;
    }
    return true;
}

bool File::PrintRawData() const {
    for (const auto& line : raw_data_) std::cout << line << '\n';
    return true;
}

bool File::PrintInstruction(uint64_t address) const {
    if (!instructions_.count(address)) {
        PrintErrorMessage(3);
        return false;
    }
    for (const auto& code : instructions_.at(address)) std::cout << code << ' ';
    std::cout << '\n';
    return true;
}

bool File::PrintErrorMessage(const int error_code) const {
    std::cerr << "File Error ";
    switch (error_code) {
        case 1: std::cerr << "1: Cannot open file.\n"; break;
        case 2: std::cerr << "2: Invalid instruction.\n"; break;
        case 3: std::cerr << "3: Instruction not found.\n"; break;
        default: std::cerr << "X: An unknown error occurs.\n"; break;
    }
    return true;
}

}  // namespace assets
