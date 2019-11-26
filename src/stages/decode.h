#ifndef SRC_STAGES_DECODE_H_
#define SRC_STAGES_DECODE_H_

#include <cstdint>
#include <vector>

#include "../assets/file.h"
#include "../assets/register.h"

namespace stages {

class Decode {
public:
    // Runs the decode stage
    static bool Do(const assets::File& input);

protected:
    static bool PrintErrorMessage(const int error_code);

    static std::vector<uint8_t>     instruction_;
    static assets::PipelineRegister register_;
};

}  // namespace stages

#endif  // SRC_STAGES_DECODE_H_
