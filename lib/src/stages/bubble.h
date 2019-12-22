#ifndef SRC_STAGES_BUBBLE_H_
#define SRC_STAGES_BUBBLE_H_

namespace stages {

class Bubble {
public:
    // Adds bubble to each stage and updates all pipeline registers
    static bool UpdateAll();

    static bool UpdateFetchStage();
    static bool UpdateDecodeStage();
    static bool UpdateExecuteStage();
    static bool UpdateMemoryStage();
    static bool UpdateWriteBackStage();
};

}  // namespace stages

#endif  // SRC_STAGES_BUBBLE_H_
