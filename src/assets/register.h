#ifndef SRC_ASSETS_REGISTER_H_
#define SRC_ASSETS_REGISTER_H_

#include <cstdint>
#include <vector>

namespace assets {

enum RegisterMap : int {
    RAX   = 0x0,
    RCX   = 0x1,
    RDX   = 0x2,
    RBX   = 0x3,
    RSP   = 0x4,
    RBP   = 0x5,
    RSI   = 0x6,
    RDI   = 0x7,
    R8    = 0x8,
    R9    = 0x9,
    R10   = 0xA,
    R11   = 0xB,
    R12   = 0xC,
    R13   = 0xD,
    R14   = 0xE,
    RNONE = 0xF
};

enum PipelineRegisterMap : int {
    PRED_PC = 0x0,
    STAT    = 0x1,
    I_CODE  = 0x2,
    I_FUN   = 0x3,
    R_A     = 0x4,
    R_B     = 0x5,
    VAL_C   = 0x6,
    VAL_P   = 0x7,
    VAL_A   = 0x8,
    VAL_B   = 0x9,
    VAL_E   = 0xA,
    VAL_M   = 0xB,
    SRC_A   = 0xC,
    SRC_B   = 0xD,
    DST_E   = 0xE,
    DST_M   = 0xF
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

// Manages the value in each pipeline register
class PipelineRegister : public Register {
protected:
    static constexpr size_t kTotal_ = 16;
};

// Manages the value in each condition code
class ConditionCode : public Register {
protected:
    static constexpr size_t kTotal_ = 3;
};

}  // namespace assets

#endif  // SRC_ASSETS_REGISTER_H_
