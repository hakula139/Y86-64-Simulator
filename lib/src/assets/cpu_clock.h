#ifndef SRC_ASSETS_CPU_CLOCK_H_
#define SRC_ASSETS_CPU_CLOCK_H_

#include <cstdint>

namespace assets {

// The CPU clock is required when generating a json file (to record changes),
// thus being declared as a global variable.
extern uint64_t cpu_clock;

}  // namespace assets

#endif  // SRC_ASSETS_CPU_CLOCK_H_
