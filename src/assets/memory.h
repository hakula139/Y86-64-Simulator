#ifndef SRC_ASSETS_MEMORY_H_
#define SRC_ASSETS_MEMORY_H_

#include <cstdint>
#include <vector>

namespace assets {

// Manages the memory, using little-endian storage
class Memory {
public:
    Memory() : data_(kCapacity_) {}

    // Reads 'size' (<= 8) bytes from 'address'
    uint64_t Get(uint64_t address, size_t size) const;
    // Writes 'value' (8 bytes) to 'address'
    bool Set(uint64_t address, uint64_t value);
    bool Clear();

protected:
    static constexpr size_t kCapacity_ = 1 << 28;  // 256 MB
    std::vector<uint8_t>    data_;
};

}  // namespace assets

#endif  // SRC_ASSETS_MEMORY_H_
