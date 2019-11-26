#include "file.h"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <utility>

#include "../utils/utility.h"

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
    for (const auto& line : raw_data_) {
        if (line.empty() || line.front() != '0') continue;
        std::vector<uint8_t> instruction;
        auto                 begin = line.find(':') + 2;
        auto                 i     = begin;
        for (; line[i] != ' '; i += 2) {
            auto code_str = line.substr(i, 2);
            auto code = static_cast<uint8_t>(std::stoi(code_str, nullptr, 16));
            instruction.push_back(code);
        }
        if (instruction.empty()) continue;  // no instruction extracted
        auto address_str = line.substr(0, begin - 2);
        auto address =
            static_cast<uint64_t>(std::stoi(address_str, nullptr, 16));
        instructions_.insert(std::make_pair(address, instruction));
    }
    return true;
}

bool File::PrintRawData() const {
    for (const auto& line : raw_data_) std::cout << line << '\n';
    return true;
}

std::vector<uint8_t> File::GetInstruction(uint64_t address,
                                          bool*    mem_error) const {
    if (!instructions_.count(address)) {
        PrintErrorMessage(3);
        *mem_error = true;
        return {};
    }
    return instructions_.at(address);
}

bool File::PrintInstruction(uint64_t address, bool* mem_error) const {
    if (!instructions_.count(address)) {
        PrintErrorMessage(3);
        *mem_error = true;
        return false;
    }
    std::cout << "0x";
    utility::SetOutputHexWidth(3);
    std::cout << address << ':';
    for (const auto& code : instructions_.at(address)) {
        std::cout << ' ';
        utility::SetOutputHexWidth(2);
        std::cout << static_cast<uint16_t>(code);
    }
    std::cout << '\n';
    return true;
}

bool File::PrintAllInstructions() const {
    for (const auto& instruction : instructions_)
        PrintInstruction(instruction.first);
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
