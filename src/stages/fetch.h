#ifndef SRC_STAGES_FETCH_H_
#define SRC_STAGES_FETCH_H_

#include <cstdint>
#include <vector>

#include "../assets/file.h"
#include "../assets/register.h"

namespace stages {

class Fetch {
public:
    static bool    Do(const assets::File& input);
    static uint8_t GetICode();
    static uint8_t GetIFun();
    static bool    InstructionIsValid(uint8_t icode);

protected:
    static bool PrintErrorMessage(const int error_code);

    static std::vector<uint8_t>     instruction_;
    static assets::PipelineRegister register_;
};

}  // namespace stages

#endif  // SRC_STAGES_FETCH_H_
