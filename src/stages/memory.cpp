#include "memory.h"

#include <iostream>
#include <utility>

#include "../assets/file.h"
#include "../assets/register.h"
#include "../utils/utility.h"
#include "instruction.h"

using std::move;
using std::vector;

using assets::File;
using assets::PipelineRegister;
using assets::ProgramCounter;
using utility::ValueIsInArray;

using assets::DECODE;
using assets::EXECUTE;
using assets::MEMORY;

namespace stages {

bool Memory::Do() {
    auto stat  = PipelineRegister::Get(MEMORY, assets::STAT);
    auto dst_e = PipelineRegister::Get(MEMORY, assets::DST_E);
    auto dst_m = PipelineRegister::Get(MEMORY, assets::DST_M);
    auto icode = PipelineRegister::Get(MEMORY, assets::I_CODE);
    auto val_e = PipelineRegister::Get(MEMORY, assets::VAL_E);
    auto val_a = PipelineRegister::Get(MEMORY, assets::VAL_A);
    auto val_m = GetMemAddr(icode);

    if (!dmem_error)
        PipelineRegister::Set(WRITEBACK, assets::STAT, stat);
    else
        PipelineRegister::Set(WRITEBACK, assets::STAT, SADR);
    PipelineRegister::Set(WRITEBACK, assets::DST_E, dst_e);
    PipelineRegister::Set(WRITEBACK, assets::DST_M, dst_m);
    PipelineRegister::Set(WRITEBACK, assets::I_CODE, icode);
    PipelineRegister::Set(WRITEBACK, assets::VAL_E, val_e);
    PipelineRegister::Set(WRITEBACK, assets::VAL_M, val_m);
}

bool Memory::GetMemRead(uint8_t icode) {
    return ValueIsInArray(icode, {IMRMOVQ, IPOPQ, IRET});
}
bool Memory::GetMemRead(uint8_t icode) {
    return ValueIsInArray(icode, {IMRMOVQ, IPUSHQ, ICALL});
}

bool Memory::PrintErrorMessage(const int error_code) {
    std::cerr << "Execute Error ";
    switch (error_code) {
        default: std::cerr << "X: An unknown error occurs.\n"; break;
    }
    return true;
}

uint64_t GetMemAddr(uint8_t icode) {
    if (ValueIsInArray(icode, {IRMMOVQ, IMRMOVQ, ICALL, IPUSHQ}))
        return PipelineRegister::Get(MEMORY, assets::VAL_E);
    if (ValueIsInArray(icode, {IPOPQ, IRET}))
        return PipelineRegister::Get(MEMORY, assets::VAL_A);
}

}  // namespace stages