#ifndef SRC_ASSETS_MEMORY_H_
#define SRC_ASSETS_MEMORY_H_

#include <cstdint>
#include <vector>

namespace assets {

// Manages the memory, using little-endian storage
class Memory {
public:
    // Reads 'size' (<= 8) bytes from 'address'
    static uint64_t Get(uint64_t address, size_t size);
    // Writes 'value' (8 bytes) to 'address'
    static bool Set(uint64_t address, uint64_t value);
    static bool Clear();

protected:
    static bool PrintErrorMessage(const int error_code);

    static constexpr size_t     kCapacity_ = 1 << 28;  // 256 MB
    static std::vector<uint8_t> data_;
};

}  // namespace assets

#endif  // SRC_ASSETS_MEMORY_H_
