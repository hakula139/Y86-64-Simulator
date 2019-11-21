#include "memory.h"

#include <iostream>

namespace assets {

std::vector<uint8_t> Memory::data_(kCapacity_);

uint64_t Memory::Get(uint64_t address, size_t size) {
    if (size > 8 || address >= kCapacity_ - size) {
        PrintErrorMessage(1);
        return UINT64_MAX;
    }
    uint64_t result = 0;
    for (size_t i = 0; i < size; ++i)
        result += data_.at(i + address) << (i << 3);
    return result;
}

bool Memory::Set(uint64_t address, uint64_t value) {
    if (address >= kCapacity_ - 8) {
        PrintErrorMessage(1);
        return false;
    }
    for (; value; value >>= 8)
        data_.at(address++) = value & 0xFF;  // gets the lowest byte
    return true;
}

bool Memory::Clear() {
    for (auto&& value : data_) value = 0;
    return true;
}

bool Memory::PrintErrorMessage(const int error_code) {
    std::cerr << "Memory Error ";
    switch (error_code) {
        case 1: std::cerr << "1: Out of range.\n"; break;
        default: std::cerr << "X: An unknown error occurs.\n"; break;
    }
    return true;
}

}  // namespace assets
