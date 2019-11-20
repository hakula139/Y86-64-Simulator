#include "memory.h"

namespace assets {

uint64_t Memory::Get(uint64_t address, size_t size) const {
    if (address >= kCapacity_ - size) return UINT64_MAX;  // error
    uint64_t result = 0;
    for (size_t i = 0; i < size; ++i)
        result += data_.at(i + address) << (i << 3);
    return result;
}

bool Memory::Set(uint64_t address, uint64_t value) {
    if (address >= kCapacity_ - 8) return false;
    for (; value; value >> 8)
        data_.at(address++) = value & 0xFF;  // gets the lowest byte
    return true;
}

bool Memory::Clear() {
    for (auto&& value : data_) value = 0;
    return true;
}

}  // namespace assets
