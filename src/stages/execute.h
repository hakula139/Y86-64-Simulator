#ifndef SRC_STAGES_EXECUTE_H_
#define SRC_STAGES_EXECUTE_H_

#include <cstdint>
#include <vector>

#include "../assets/register.h"

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

    static uint64_t val_e() { return val_e_; }
    static uint64_t dst_e() { return dst_e_; }

protected:
    static bool PrintErrorMessage(const int error_code);

    static uint64_t val_e_, dst_e_;
};

}  // namespace stages

#endif  // SRC_STAGES_EXECUTE_H_
