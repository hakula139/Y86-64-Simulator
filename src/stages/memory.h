#ifndef SRC_STAGES_MEMORY_H_
#define SRC_STAGES_MEMORY_H_

#include <cstdint>

namespace stages {

class Memory {
public:
    // Runs the memory stage
    static bool Do();

    static uint64_t GetStat();
    // Selects memory address
    static uint64_t GetMemAddress(uint8_t icode);
    // Sets read control signal
    static bool GetMemRead(uint8_t icode);
    // Sets write control signal
    static bool GetMemWrite(uint8_t icode);

    static uint64_t stat() { return stat_; }

protected:
    static bool PrintErrorMessage(const int error_code);

    static uint64_t stat_;
};

}  // namespace stages

#endif  // SRC_STAGES_MEMORY_H_
