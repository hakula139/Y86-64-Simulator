#include "fetch.h"

#include <iostream>

#include "../assets/file.h"
#include "../assets/register.h"
#include "../utils/utility.h"
#include "instruction.h"
#include "memory.h"

using assets::File;
using assets::PipelineRegister;
using assets::ProgramCounter;
using utility::ValueIsInArray;

using assets::DECODE;
using assets::EXECUTE;
using assets::FETCH;
using assets::MEMORY;
using assets::WRITE_BACK;

namespace stages {

std::vector<uint8_t> Fetch::instruction_;
uint64_t             Fetch::pc_;
uint64_t             Fetch::pred_pc_;
uint8_t              Fetch::icode_;
uint8_t              Fetch::ifun_;
uint8_t              Fetch::stat_;
bool                 Fetch::mem_error_ = false;

bool Fetch::Do(const File& input) {
    pc_          = GetPC();
    instruction_ = input.GetInstruction(pc_, &mem_error_);
    icode_       = GetICode();
    ifun_        = GetIFun();
    stat_        = GetStat();

    PipelineRegister::Set(DECODE, assets::I_CODE, icode_);
    PipelineRegister::Set(DECODE, assets::I_FUN, ifun_);
    PipelineRegister::Set(DECODE, assets::STAT, stat_);
    ++pc_;

    if (NeedRegids()) {
        PipelineRegister::Set(DECODE, assets::R_A, GetRA());
        PipelineRegister::Set(DECODE, assets::R_B, GetRB());
        ++pc_;
    }
    if (NeedValC()) {
        PipelineRegister::Set(DECODE, assets::VAL_C, GetValC());
        pc_ += 8;
    }

    PipelineRegister::Set(DECODE, assets::VAL_P, pc_);
    return true;
}

uint64_t Fetch::GetPC() {
    // Mispredicted branch: Fetches at incremented PC.
    auto m_icode = PipelineRegister::Get(MEMORY, assets::I_CODE);
    auto m_cnd   = PipelineRegister::Get(MEMORY, assets::CND);
    if (m_icode == IJXX && !m_cnd)
        return PipelineRegister::Get(MEMORY, assets::VAL_A);
    // Completion of RET instruction
    auto w_icode = PipelineRegister::Get(WRITE_BACK, assets::I_CODE);
    if (w_icode == IRET)
        return PipelineRegister::Get(WRITE_BACK, assets::VAL_M);
    //  Default: Uses predicted value of PC
    return PipelineRegister::Get(FETCH, assets::PRED_PC);
}

uint8_t Fetch::GetICode() {
    if (mem_error_) return INOP;
    if (instruction_.empty()) {
        Fetch::PrintErrorMessage(1);
        mem_error_ = true;
        return INOP;
    }
    return (instruction_[0] >> 4) & 0xF;  // the first 4 bits
}

uint8_t Fetch::GetIFun() {
    if (mem_error_) return FNONE;
    return instruction_[0] & 0xF;  // the second 4 bits
}

uint8_t Fetch::GetStat() {
    if (mem_error_) return assets::SADR;
    auto ins_valid = InstructionIsValid();
    if (!ins_valid) {
        Fetch::PrintErrorMessage(2);
        return assets::SINS;
    }
    if (icode_ == IHALT) return assets::SHLT;
    return assets::SAOK;
}

uint8_t Fetch::GetRA() { return (instruction_[1] >> 4) & 0xF; }

uint8_t Fetch::GetRB() { return instruction_[1] & 0xF; }

uint64_t Fetch::GetValC() {
    uint64_t val_c = 0;
    // Read 8 bytes from instruction
    for (size_t i = 2; i < 10; ++i) val_c += instruction_[i] << ((i - 2) << 3);
    return val_c;
}

bool Fetch::InstructionIsValid() {
    return ValueIsInArray(icode_,
                          {IHALT, INOP, IRRMOVQ, IIRMOVQ, IRMMOVQ, IMRMOVQ,
                           IOPQ, IJXX, ICALL, IRET, IPUSHQ, IPOPQ});
}

bool Fetch::NeedRegids() {
    return ValueIsInArray(icode_, {IRRMOVQ, IOPQ, IPUSHQ, IPOPQ, IIRMOVQ,
                                   IRMMOVQ, IMRMOVQ});
}

bool Fetch::NeedValC() {
    return ValueIsInArray(icode_, {IIRMOVQ, IRMMOVQ, IMRMOVQ, IJXX, ICALL});
}

bool Fetch::PrintErrorMessage(const int error_code) {
    std::cerr << "Fetch Error ";
    switch (error_code) {
        case 1: std::cerr << "1: No instruction.\n"; break;
        case 2: std::cerr << "2: Invalid instruction.\n"; break;
        default: std::cerr << "X: An unknown error occurs.\n"; break;
    }
    return true;
}

}  // namespace stages
