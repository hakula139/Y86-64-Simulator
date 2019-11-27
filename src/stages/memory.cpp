#include "memory.h"

#include <iostream>

#include "../assets/file.h"
#include "../assets/memory.h"
#include "../assets/register.h"
#include "../utils/utility.h"
#include "instruction.h"
#include "write_back.h"

using assets::PipelineRegister;
using utility::ValueIsInArray;

using assets::DECODE;
using assets::EXECUTE;
using assets::FETCH;
using assets::MEMORY;
using assets::WRITE_BACK;

namespace stages {

uint8_t  Memory::stat_;
uint64_t Memory::val_m_;
bool     Memory::mem_error_ = false;

bool Memory::Do() {
    stat_        = PipelineRegister::Get(MEMORY, assets::STAT);
    auto icode   = PipelineRegister::Get(MEMORY, assets::I_CODE);
    auto val_e   = PipelineRegister::Get(MEMORY, assets::VAL_E);
    auto val_a   = PipelineRegister::Get(MEMORY, assets::VAL_A);
    auto dst_e   = PipelineRegister::Get(MEMORY, assets::DST_E);
    auto dst_m   = PipelineRegister::Get(MEMORY, assets::DST_M);
    auto address = GetMemAddress(icode);
    if (GetMemRead(icode))
        val_m_ = assets::Memory::Get(address, 8, &mem_error_);
    else
        val_m_ = val_a;
    if (GetMemWrite(icode)) assets::Memory::Set(address, val_a, &mem_error_);

    if (WriteBack::NeedBubble()) {
        PipelineRegister::Clear(WRITE_BACK);
    } else if (!WriteBack::NeedStall()) {
        PipelineRegister::Set(WRITE_BACK, assets::STAT, GetStat());
        PipelineRegister::Set(WRITE_BACK, assets::I_CODE, icode);
        PipelineRegister::Set(WRITE_BACK, assets::VAL_E, val_e);
        PipelineRegister::Set(WRITE_BACK, assets::VAL_M, val_m_);
        PipelineRegister::Set(WRITE_BACK, assets::DST_E, dst_e);
        PipelineRegister::Set(WRITE_BACK, assets::DST_M, dst_m);
    }
    return true;
}

uint8_t Memory::GetStat() {
    if (mem_error_) {
        PrintErrorMessage(1);
        return assets::SADR;
    }
    return stat_;
}

uint64_t Memory::GetMemAddress(uint8_t icode) {
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
    return ValueIsInArray(icode, {IRMMOVQ, IPUSHQ, ICALL});
}

bool Memory::NeedBubble() {
    // Starts injecting bubbles as soon as exception passes through memory stage
    if (ValueIsInArray(stat_, {assets::SADR, assets::SINS, assets::SHLT}))
        return true;
    auto w_stat = PipelineRegister::Get(WRITE_BACK, assets::STAT);
    if (ValueIsInArray(w_stat, {assets::SADR, assets::SINS, assets::SHLT}))
        return true;
    return false;
}

bool Memory::NeedStall() { return false; }

bool Memory::PrintErrorMessage(const int error_code) {
    std::cerr << "Memory Error ";
    switch (error_code) {
        case 1: std::cerr << "1: Cannot operate memory.\n"; break;
        default: std::cerr << "X: An unknown error occurs.\n"; break;
    }
    return true;
}

}  // namespace stages
