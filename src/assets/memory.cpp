#include "memory.h"

#include <fstream>
#include <iostream>

#include "../utils/utility.h"

namespace assets {

std::vector<uint8_t> Memory::data_(kCapacity_);

uint64_t Memory::Get(uint64_t address, size_t size, bool* mem_error) {
    if (size > 8 || address >= kCapacity_ - size) {
        PrintErrorMessage(1);
        *mem_error = true;
        return UINT64_MAX;
    }
    uint64_t result = 0ull;
    for (size_t i = 0; i < size; ++i)
        result |= static_cast<uint64_t>(data_.at(address++)) << (i << 3);
    return result;
}

bool Memory::Set(uint64_t address,
                 uint64_t value,
                 size_t   size,
                 bool*    mem_error) {
    if (size > 8 || address >= kCapacity_ - size) {
        PrintErrorMessage(1);
        *mem_error = true;
        return false;
    }
    for (size_t i = 0; i < size; ++i, value >>= 8)
        data_.at(address++) = value & 0xFF;  // gets the lowest byte
    return true;
}

bool Memory::Clear() {
    for (auto&& value : data_) value = 0;
    return true;
}

bool Memory::Dump() {
    std::ofstream output;
    output.open("memory.txt", std::ofstream::out | std::ofstream::trunc);
    if (!output) {
        PrintErrorMessage(2);
        return false;
    }
    output << "===== MEMORY DUMP =====";
    uint64_t address = 0ull;
    for (auto&& value : data_) {
        if (!(address % 8)) {
            output << "\n0x";
            utility::SetOutputHexWidth(3, output);
            output << address << ": ";
        }
        utility::SetOutputHexWidth(2, output);
        output << static_cast<int>(value) << ' ';
        ++address;
    }
    output << '\n';
    output.close();
    return true;
}

bool Memory::PrintErrorMessage(const int error_code) {
    std::cerr << "Memory Error ";
    switch (error_code) {
        case 1: std::cerr << "1: Out of range.\n"; break;
        case 2: std::cerr << "2: Cannot write file.\n"; break;
        default: std::cerr << "X: An unknown error occurs.\n"; break;
    }
    return true;
}

}  // namespace assets
