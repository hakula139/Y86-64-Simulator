#ifndef SRC_STAGES_STAGE_H_
#define SRC_STAGES_STAGE_H_

#include "../assets/register.h"

namespace stages {

class Stage {
public:
    virtual assets::PipelineRegister GetRegister()   = 0;
    virtual bool                     ClearRegister() = 0;
};

}  // namespace stages

#endif  // SRC_STAGES_STAGE_H_
