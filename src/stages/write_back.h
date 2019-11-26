#ifndef SRC_STAGES_WRITE_BACK_H_
#define SRC_STAGES_WRITE_BACK_H_

#include <cstdint>
#include <vector>

#include "../assets/register.h"

namespace stages {

class WriteBack {
public:
    // Runs the writeback stage
    static bool Do();

protected:
    static bool PrintErrorMessage(const int error_code);
};

};  // namespace stages

#endif  // SRC_STAGES_WRITE_BACK_H_
