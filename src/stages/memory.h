#ifndef SRC_STAGES_MEMORY_H_
#define SRC_STAGES_MEMORY_H_

#include <cstdint>

namespace stages {

class Memory {
public:
    // Runs the memory stage
    static bool Do();

    static uint8_t GetStat();
    // Selects memory address
    static uint64_t GetMemAddress(uint8_t icode);
    // Sets read control signal
    static bool GetMemRead(uint8_t icode);
    // Sets write control signal
    static bool GetMemWrite(uint8_t icode);

    // Should I stall or inject a bubble into Pipeline Register M?
    static bool NeedBubble();
    static bool NeedStall();

    static uint8_t  stat() { return stat_; }
    static uint64_t val_m() { return val_m_; }
    static bool     mem_error() { return mem_error_; }

protected:
    static bool PrintErrorMessage(const int error_code);

    static uint8_t  stat_;
    static uint64_t val_m_;
    static bool     mem_error_;
};

}  // namespace stages

#endif  // SRC_STAGES_MEMORY_H_
