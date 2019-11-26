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
    static bool     Do(const assets::File& input);
    static uint64_t GetAluA(uint8_t icode);
    static uint64_t GetAluB(uint8_t icode);
    static uint64_t GetSrcA(uint8_t icode);
    static uint64_t GetSrcB(uint8_t icode);
    static uint64_t GetDstE(uint8_t icode);
    static uint64_t GetDstM(uint8_t icode);
    static uint64_t SrcA(void);
    static uint64_t SrcB(void);
    static uint64_t DstE(void);
    static uint64_t DstM(void);


protected:
    static bool PrintErrorMessage(const int error_code);

    static std::vector<uint8_t>     instruction_;
    static assets::PipelineRegister register_;

    static uint64_t srcA, srcB, dstE, dstM;
};

}  // namespace stages

#endif  // SRC_STAGES_DECODE_H_
