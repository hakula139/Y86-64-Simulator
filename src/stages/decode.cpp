#include "decode.h"

#include <iostream>

#include "../assets/file.h"
#include "../assets/register.h"
#include "../utils/utility.h"
#include "instruction.h"

using utility::ValueIsInArray;

namespace stages {

bool Decode::Do(const assets::File& input) {
    auto stat   = assets::PipelineRegister::Get(assets::DECODE, assets::STAT);
    auto icode  = assets::PipelineRegister::Get(assets::DECODE, assets::I_CODE);
    auto ifun   = assets::PipelineRegister::Get(assets::DECODE, assets::I_FUN);
    auto val_c  = assets::PipelineRegister::Get(assets::DECODE, assets::VAL_C);
    auto val_p  = assets::PipelineRegister::Get(assets::FETCH, assets::PRED_PC);
    auto val_a  = GetAluA(icode);
    auto val_b  = GetAluB(icode);
    auto M_valE = assets::PipelineRegister::Get(assets::MEMORY, assets::VAL_E);
    auto W_valM =
        assets::PipelineRegister::Get(assets::WRITE_BACK, assets::VAL_M);
    auto W_valE =
        assets::PipelineRegister::Get(assets::WRITE_BACK, assets::VAL_E);

    srcA = GetSrcA(icode);
    srcB = GetSrcB(icode);
    dstE = GetDstE(icode);
    dstM = GetDstM(icode);

    assets::PipelineRegister::Set(assets::EXECUTE, assets::STAT, stat);
    assets::PipelineRegister::Set(assets::EXECUTE, assets::I_CODE, icode);
    assets::PipelineRegister::Set(assets::EXECUTE, assets::I_FUN, ifun);
    assets::PipelineRegister::Set(assets::EXECUTE, assets::VAL_C, val_c);
    assets::PipelineRegister::Set(assets::EXECUTE, assets::VAL_A, val_a);
    assets::PipelineRegister::Set(assets::EXECUTE, assets::VAL_B, val_b);

    return true;
}

bool Decode::PrintErrorMessage(const int error_code) {
    std::cerr << "Fetch Error ";
    switch (error_code) {
        case 1: std::cerr << "1: No instruction.\n"; break;
        case 2: std::cerr << "2: Invalid instruction.\n"; break;
        default: std::cerr << "X: An unknown error occurs.\n"; break;
    }
    return true;
}

uint64_t Decode::GetAluA(uint8_t icode) {
    if (ValueIsInArray(icode, {ICALL, IJXX}))
        return assets::PipelineRegister::Get(assets::DECODE, assets::VAL_A);
    if (srcA == excecute::DstE())
        return e_valE;  // Forward valE from execute     :
    if (srcA == MEMORY::DstM()) return m_valM;  // Forward valM from memory
    if (srcA == MEMORY::DstE()) return M_valE;  // Forward valE from memory
    if (srcA == WRITE_BACK::DstM())
        return W_valM;  // Forward valM from write back
    if (srcA == WRITE_BACK::DstE)
        return W_valE;  // Forward valE from write back
    return d_rvalA;     // Use value read from register file
}

uint64_t Decode::GetAluB(uint8_t icode) {
    if (srcB == excecute::DstE())
        return e_valE;  // Forward valE from execute     :
    if (srcB == MEMORY::DstM()) return m_valM;  // Forward valM from memory
    if (srcB == MEMORY::DstE()) return M_valE;  // Forward valE from memory
    if (srcB == WRITE_BACK::DstM())
        return W_valM;  // Forward valM from write back
    if (srcB == WRITE_BACK::DstE())
        return W_valE;  // Forward valE from write back
    return d_rvalB;     // Use value read from register file
}

uint64_t Decode::GetSrcA(uint8_t icode) {
    if (ValueIsInArray(icode, {IRRMOVQ, IRMMOVQ, IOPQ, IPUSHQ}))
        return assets::PipelineRegister::Get(assets::DECODE, assets::R_A);
    if (ValueIsInArray(icode, {IPOPQ, IRET}))
        return RRSP;
    return RNONE;
}

uint64_t Decode::GetSrcB(uint8_t icode) {
    if (ValueIsInArray(icode, {IMRMOVQ, IRMMOVQ, IOPQ}))
        return assets::PipelineRegister::Get(assets::DECODE, assets::R_B);
    if (ValueIsInArray(icode, {IPUSHQ, IPOPQ, ICALL, IRET}))
        return RRSP;
    return RNONE;
}

uint64_t Decode::GetDstE(uint8_t icode) {
    if (ValueIsInArray(icode, {IRRMOVQ, IIRMOVQ, IOPQ}))
        return assets::PipelineRegister::Get(assets::DECODE, assets::R_B);
    if (ValueIsInArray(icode, {IPUSHQ, IPOPQ, ICALL, IRET}))
        return RRSP;
    return RNONE;
}

uint64_t Decode::GetDstM(uint8_t icode) {
    if (ValueIsInArray(icode, {IMRMOVQ, IOPQ}))
        return assets::PipelineRegister::Get(assets::DECODE, assets::R_A);
    return RNONE;
}

uint64_t Decode::SrcA(void) { return srcA; }
uint64_t Decode::SrcB(void) { return srcB; }
uint64_t Decode::DstE(void) { return dstE; }
uint64_t Decode::DstM(void) { return dstM; }

}  // namespace stages
