#ifndef SRC_STAGES_WRITE_BACK_H_
#define SRC_STAGES_WRITE_BACK_H_

#include <cstdint>

namespace stages {

class WriteBack {
public:
    // Runs the writeback stage
    static uint8_t Do();
    static uint8_t GetStat();

    // Should I stall or inject a bubble into Pipeline Register W?
    static bool NeedBubble();
    static bool NeedStall();

    static uint8_t stat() { return stat_; }
    static bool    bubble() { return bubble_; }
    static bool    stall() { return stall_; }

    static void set_bubble(bool bubble) { bubble_ = bubble; }
    static void set_stall(bool stall) { stall_ = stall; }

protected:
    static bool PrintErrorMessage(const int error_code);

    static uint8_t stat_;
    static bool    bubble_;
    static bool    stall_;
};

};  // namespace stages

#endif  // SRC_STAGES_WRITE_BACK_H_
