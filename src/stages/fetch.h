#ifndef SRC_STAGES_FETCH_H_
#define SRC_STAGES_FETCH_H_

#include <cstdint>
#include <vector>

#include "../assets/file.h"
#include "../assets/register.h"

namespace stages {

class Fetch {
public:
    // Runs the fetch stage
    static bool Do(const assets::File& input);

    static uint64_t GetPC();
    static uint8_t  GetICode();
    static uint8_t  GetIFun();
    static uint8_t  GetStat();
    static uint8_t  GetRA();
    static uint8_t  GetRB();
    static uint64_t GetValC();

    static bool InstructionIsValid();
    // Does fetched instruction require a regid byte?
    static bool NeedRegids();
    // Does fetched instruction require a constant word?
    static bool NeedValC();

    static uint64_t pc() { return pc_; }
    static uint64_t pred_pc() { return pred_pc_; }
    static uint8_t  icode() { return icode_; }
    static uint8_t  ifun() { return ifun_; }
    static uint8_t  stat() { return stat_; }
    static bool     mem_error() { return mem_error_; }

protected:
    static bool PrintErrorMessage(const int error_code);

    static std::vector<uint8_t> instruction_;
    static uint64_t             pc_, pred_pc_;
    static uint8_t              icode_, ifun_, stat_;
    static bool                 mem_error_;
};

}  // namespace stages

#endif  // SRC_STAGES_FETCH_H_
