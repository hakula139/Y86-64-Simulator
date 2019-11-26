#ifndef SRC_STAGES_FETCH_H_
#define SRC_STAGES_FETCH_H_

#include "../assets/register.h"

namespace stages {

class Fetch {
public:
    static bool InstructionIsValid();

protected:
    static assets::PipelineRegister register_;
};

}  // namespace stages

#endif  // SRC_STAGES_FETCH_H_
