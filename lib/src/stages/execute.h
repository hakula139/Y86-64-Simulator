#ifndef SRC_STAGES_EXECUTE_H_
#define SRC_STAGES_EXECUTE_H_

#include <cstdint>

namespace stages {

class Execute {
public:
    friend class Bubble;

    // Runs the execute stage
    static bool Do();

    // Selects input A to ALU
    static uint64_t GetAluA();
    // Selects input B to ALU
    static uint64_t GetAluB();
    // Sets the ALU function
    static uint64_t GetAluFunction();
    static bool     GetCondition();
    static uint64_t GetValE(uint64_t alu_a, uint64_t alu_b, uint64_t alu_func);
    static uint64_t GetDstE();

    // True if the condition codes should be updated
    static bool NeedUpdateCC();
    // Should I stall or inject a bubble into Pipeline Register E?
    static bool NeedBubble();
    static bool NeedStall();

    static uint64_t val_a() { return val_a_; }
    static uint64_t val_e() { return val_e_; }
    static uint64_t dst_e() { return dst_e_; }
    static bool     cnd() { return cnd_; }

protected:
    static bool PrintErrorMessage(const int error_code);

    static uint64_t val_a_, val_e_, dst_e_, dst_m_;
    static uint8_t  icode_, ifun_, stat_;
    static bool     cnd_;
};

}  // namespace stages

#endif  // SRC_STAGES_EXECUTE_H_
