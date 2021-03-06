#ifndef SRC_STAGES_FETCH_H_
#define SRC_STAGES_FETCH_H_

#include <cstdint>
#include <vector>

#include "../assets/file.h"

namespace stages {

class Fetch {
public:
    friend class Bubble;

    // Runs the fetch stage
    static bool Do(const assets::File& input);

    // What address should instruction be fetched at
    static uint64_t GetPC();
    // Predict next value of PC
    static uint64_t GetPredPC();
    // Determines icode of fetched instruction
    static uint8_t GetICode();
    // Determines ifun
    static uint8_t GetIFun();
    // Determines status code for fetched instruction
    static uint8_t GetStat();
    static uint8_t GetRA();
    static uint8_t GetRB();
    // Does fetched instruction require a constant word?
    static uint64_t GetValC(uint64_t pos);

    static bool InstructionIsValid();
    // Does fetched instruction require a regid byte?
    static bool NeedRegids();
    // Does fetched instruction require a constant word?
    static bool NeedValC();
    // Should I stall or inject a bubble into Pipeline Register F?
    static bool NeedBubble();
    static bool NeedStall();

    static bool mem_error() { return mem_error_; }
    static bool reach_eof() { return reach_eof_; }

protected:
    static bool PrintErrorMessage(const int error_code);

    static std::vector<uint8_t> instruction_;
    static uint64_t             pc_, pred_pc_, val_c_;
    static uint8_t              icode_, ifun_, stat_, r_a_, r_b_;
    static bool                 mem_error_, reach_eof_;
};

}  // namespace stages

#endif  // SRC_STAGES_FETCH_H_
