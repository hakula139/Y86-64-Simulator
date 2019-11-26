#ifndef SRC_STAGES_INSTRUCTION_H_
#define SRC_STAGES_INSTRUCTION_H_

namespace stages {

enum InstructionMap : uint8_t {
    IHALT   = 0x0,
    INOP    = 0x1,
    IRRMOVQ = 0x2,
    IIRMOVQ = 0x3,
    IRMMOVQ = 0x4,
    IMRMOVQ = 0x5,
    IOPQ    = 0x6,
    IJXX    = 0x7,
    ICALL   = 0x8,
    IRET    = 0x9,
    IPUSHQ  = 0xA,
    IPOPQ   = 0xB
};

constexpr uint8_t FNONE = 0x0;

}  // namespace stages

#endif  // SRC_STAGES_INSTRUCTION_H_
