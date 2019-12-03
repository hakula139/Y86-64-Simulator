#ifndef SRC_STAGES_DECODE_H_
#define SRC_STAGES_DECODE_H_

#include <cstdint>

namespace stages {

class Decode {
public:
    friend class Bubble;

    // Runs the decode stage
    static bool Do();

    static uint64_t GetValA();
    static uint64_t GetValB();
    static uint64_t GetSrcA();
    static uint64_t GetSrcB();
    static uint64_t GetDstE();
    static uint64_t GetDstM();

    // Should I stall or inject a bubble into Pipeline Register D?
    static bool NeedBubble();
    static bool NeedStall();

    static uint64_t dst_e() { return dst_e_; }
    static uint64_t dst_m() { return dst_m_; }
    static uint64_t src_a() { return src_a_; }
    static uint64_t src_b() { return src_b_; }

protected:
    static bool PrintErrorMessage(const int error_code);

    static uint64_t dst_e_, dst_m_, src_a_, src_b_, val_c_, val_a_, val_b_;
    static uint8_t  icode_, ifun_, stat_;
};

}  // namespace stages

#endif  // SRC_STAGES_DECODE_H_
