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

    static uint8_t  stat() { return stat_; }
    static uint64_t val_m() { return val_m_; }
    static bool     mem_error() { return mem_error_; }

    static void set_bubble(bool bubble) { bubble_ = bubble; }
    static void set_stall(bool stall) { stall_ = stall; }

protected:
    static bool PrintErrorMessage(const int error_code);

    static uint8_t  stat_;
    static uint64_t val_m_;
    static bool     mem_error_;
    static bool     bubble_;
    static bool     stall_;
};

}  // namespace stages

#endif  // SRC_STAGES_MEMORY_H_
