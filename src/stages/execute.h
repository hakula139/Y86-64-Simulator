#ifndef SRC_STAGES_EXECUTE_H_
#define SRC_STAGES_EXECUTE_H_

#include <cstdint>

namespace stages {

class Execute {
public:
    // Runs the execute stage
    static bool Do();

    // Selects input A to ALU
    static uint64_t GetAluA(uint8_t icode);
    // Selects input B to ALU
    static uint64_t GetAluB(uint8_t icode);
    // Sets the ALU function
    static uint64_t GetAluFunction(uint8_t icode);
    static bool     GetCondition(uint8_t ifun);
    static uint64_t GetValE(uint64_t alu_a, uint64_t alu_b, uint64_t alu_func);

    // True if the condition codes should be updated
    static bool NeedUpdateCC(uint8_t icode);
    // Should I stall or inject a bubble into Pipeline Register E?
    static bool NeedBubble();
    static bool NeedStall();

    static uint64_t val_a() { return val_a_; }
    static uint64_t val_e() { return val_e_; }
    static uint64_t dst_e() { return dst_e_; }

    static void set_bubble(bool bubble) { bubble_ = bubble; }
    static void set_stall(bool stall) { stall_ = stall; }

protected:
    static bool PrintErrorMessage(const int error_code);

    static uint64_t val_a_, val_e_, dst_e_;
    static bool     bubble_;
    static bool     stall_;
};

}  // namespace stages

#endif  // SRC_STAGES_EXECUTE_H_
