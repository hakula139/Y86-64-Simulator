#include "memory.h"

#include <iostream>

#include "../assets/memory.h"
#include "../assets/register.h"
#include "../utils/utility.h"
#include "fetch.h"
#include "instruction.h"

using assets::PipelineRegister;
using utility::ValueIsInArray;

using assets::DECODE;
using assets::EXECUTE;
using assets::FETCH;
using assets::MEMORY;
using assets::WRITE_BACK;

namespace stages {

uint64_t Memory::val_m_;
uint64_t Memory::val_e_;
uint64_t Memory::val_a_;
uint64_t Memory::dst_e_;
uint64_t Memory::dst_m_;
uint8_t  Memory::icode_;
uint8_t  Memory::stat_;
bool     Memory::mem_error_ = false;

bool Memory::Do() {
    icode_       = PipelineRegister::Get(MEMORY, assets::I_CODE);
    val_e_       = PipelineRegister::Get(MEMORY, assets::VAL_E);
    val_a_       = PipelineRegister::Get(MEMORY, assets::VAL_A);
    dst_e_       = PipelineRegister::Get(MEMORY, assets::DST_E);
    dst_m_       = PipelineRegister::Get(MEMORY, assets::DST_M);
    auto address = GetMemAddress(icode_);
    if (GetMemRead(icode_))
        val_m_ = assets::Memory::Get(address, 8, &mem_error_);
    if (GetMemWrite(icode_))
        assets::Memory::Set(address, val_a_, 8, &mem_error_);
    stat_ = GetStat();
    return true;
}

uint8_t Memory::GetStat() {
    if (Fetch::mem_error() && !Fetch::reach_eof()) return assets::SADR;
    return PipelineRegister::Get(MEMORY, assets::STAT);
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

bool Memory::NeedStall() {
    return false;
}

bool Memory::PrintErrorMessage(const int error_code) {
    std::cerr << "Memory Error ";
    switch (error_code) {
    default: std::cerr << "X: An unknown error occurs.\n"; break;
    }
    return true;
}

}  // namespace stages
