#ifndef SRC_ASSETS_REGISTER_H_
#define SRC_ASSETS_REGISTER_H_

#include <cstdint>
#include <vector>

namespace assets {

enum RegisterMap : int {
    RAX = 0x0,
    RCX = 0x1,
    RDX = 0x2,
    RBX = 0x3,
    RSP = 0x4,
    RBP = 0x5,
    RSI = 0x6,
    RDI = 0x7,
    R8  = 0x8,
    R9  = 0x9,
    R10 = 0xA,
    R11 = 0xB,
    R12 = 0xC,
    R13 = 0xD,
    R14 = 0xE,
    R15 = 0xF
};

enum ConditionCodeMap : int { OF = 0x0, SF = 0x1, ZF = 0x2, CF = 0x3 };

// Manages the value in each register
class Register {
public:
    static uint64_t Get(int register_num) { return data_.at(register_num); }
    static bool     Set(int register_num, uint64_t value);
    static bool     Clear();

protected:
    static constexpr size_t      kTotal_ = 16;
    static std::vector<uint64_t> data_;
};

// Manages the value in each condition code
class ConditionCode {
public:
    static bool Get(int condition_code) { return data_.at(condition_code); }
    static bool Set(int condition_code, bool value);
    static bool Clear();

protected:
    static constexpr size_t  kTotal_ = 3;
    static std::vector<bool> data_;
};

}  // namespace assets

#endif  // SRC_ASSETS_REGISTER_H_
