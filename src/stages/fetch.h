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
    // Determines instruction code
    static uint8_t GetICode();
    // Determines instruction function
    static uint8_t GetIFun();
    static bool    InstructionIsValid(uint8_t icode);
    // Does fetched instruction require a regid byte?
    static bool NeedRegids(uint8_t icode);
    // Does fetched instruction require a constant word?
    static bool NeedValC(uint8_t icode);

protected:
    static bool PrintErrorMessage(const int error_code);

    static std::vector<uint8_t>     instruction_;
    static assets::PipelineRegister register_;
};

}  // namespace stages

#endif  // SRC_STAGES_FETCH_H_
