#include "memory.h"

#include <iostream>

#include "../assets/file.h"
#include "../assets/register.h"
#include "../utils/utility.h"
#include "instruction.h"

using assets::PipelineRegister;
using utility::ValueIsInArray;

using assets::DECODE;
using assets::EXECUTE;
using assets::FETCH;
using assets::MEMORY;
using assets::WRITE_BACK;

namespace stages {

uint8_t Memory::stat_;

bool Memory::Do() {
    stat_      = PipelineRegister::Get(MEMORY, assets::STAT);
    auto icode = PipelineRegister::Get(MEMORY, assets::I_CODE);
    auto val_e = PipelineRegister::Get(MEMORY, assets::VAL_E);
    auto val_a = PipelineRegister::Get(MEMORY, assets::VAL_A);
    auto dst_e = PipelineRegister::Get(MEMORY, assets::DST_E);
    auto dst_m = PipelineRegister::Get(MEMORY, assets::DST_M);
    auto val_m = GetMemAddress(icode);

    PipelineRegister::Set(WRITE_BACK, assets::STAT, GetStat());
    PipelineRegister::Set(WRITE_BACK, assets::I_CODE, icode);
    PipelineRegister::Set(WRITE_BACK, assets::VAL_E, val_e);
    PipelineRegister::Set(WRITE_BACK, assets::DST_E, dst_e);
    PipelineRegister::Set(WRITE_BACK, assets::DST_M, dst_m);
    PipelineRegister::Set(WRITE_BACK, assets::VAL_M, val_m);

    return true;
}

uint8_t Memory::GetStat() {
    if (dmem_error) return assets::SADR;
    return stat();
}

uint64_t GetMemAddress(uint8_t icode) {
    if (ValueIsInArray(icode, {IRMMOVQ, IMRMOVQ, ICALL, IPUSHQ}))
        return PipelineRegister::Get(MEMORY, assets::VAL_E);
    if (ValueIsInArray(icode, {IPOPQ, IRET}))
        return PipelineRegister::Get(MEMORY, assets::VAL_A);
    return 0ull;  // Other instructions don’t need address
}

bool Memory::GetMemRead(uint8_t icode) {
    return ValueIsInArray(icode, {IMRMOVQ, IPOPQ, IRET});
}

bool Memory::GetMemWrite(uint8_t icode) {
    return ValueIsInArray(icode, {IMRMOVQ, IPUSHQ, ICALL});
}

bool Memory::PrintErrorMessage(const int error_code) {
    std::cerr << "Memory Error ";
    switch (error_code) {
        default: std::cerr << "X: An unknown error occurs.\n"; break;
    }
    return true;
}

}  // namespace stages
