#ifndef SRC_STAGES_MEMORY_H_
#define SRC_STAGES_MEMORY_H_

#include <cstdint>
#include <vector>

#include "../assets/file.h"
#include "../assets/register.h"

namespace stages {

class Memory {
public:
    // Runs the memory stage
    static bool Do();

    static uint64_t GetMemAddr(uint8_t icode);
    static bool     GetMemRead(uint8_t icode);
    static bool     GetMemWrite(uint8_t icode);

protected:
    static bool PrintErrorMessage(const int error_code);
};

}  // namespace stages

#endif  // SRC_STAGES_MEMORY_H_
