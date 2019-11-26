#ifndef SRC_STAGES_EXECUTE_H_
#define SRC_STAGES_EXECUTE_H_

#include <cstdint>
#include <vector>

#include "../assets/file.h"
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

    // Should the condition codes be updated?
    static bool NeedUpdateCC(uint8_t icode);

protected:
    static bool PrintErrorMessage(const int error_code);
};

}  // namespace stages

#endif  // SRC_STAGES_EXECUTE_H_
