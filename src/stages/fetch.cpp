#include "fetch.h"

#include <iostream>

#include "../assets/file.h"
#include "../assets/register.h"
#include "../utils/utility.h"
#include "decode.h"
#include "instruction.h"

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
uint64_t             Fetch::val_c_;
uint8_t              Fetch::icode_;
uint8_t              Fetch::ifun_;
uint8_t              Fetch::stat_;
bool                 Fetch::mem_error_ = false;

bool Fetch::Do(const File& input) {
    pc_          = GetPC();
    auto cur_pc  = pc_;
    instruction_ = input.GetInstruction(pc_, &mem_error_);
    icode_       = GetICode();
    ifun_        = GetIFun();
    stat_        = GetStat();
    uint8_t r_a  = assets::RNONE;
    uint8_t r_b  = assets::RNONE;
    ++pc_;
    if (NeedRegids()) {
        r_a = GetRA();
        r_b = GetRB();
        ++pc_;
    }
    if (NeedValC()) {
        val_c_ = GetValC(pc_ - cur_pc);
        pc_ += 8;
    }

    if (NeedBubble())
        PipelineRegister::Clear(FETCH);
    else if (!NeedStall())
        PipelineRegister::Set(FETCH, assets::PRED_PC, GetPredPC());

    if (Decode::NeedBubble()) {
        PipelineRegister::Clear(DECODE);
    } else if (!Decode::NeedStall()) {
        PipelineRegister::Set(DECODE, assets::I_CODE, icode_);
        PipelineRegister::Set(DECODE, assets::I_FUN, ifun_);
        PipelineRegister::Set(DECODE, assets::STAT, stat_);
        PipelineRegister::Set(DECODE, assets::VAL_P, pc_);
        PipelineRegister::Set(DECODE, assets::R_A, r_a);
        PipelineRegister::Set(DECODE, assets::R_B, r_b);
        PipelineRegister::Set(DECODE, assets::VAL_C, val_c_);
    }
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

uint64_t Fetch::GetPredPC() {
    if (ValueIsInArray(icode_, {IJXX, ICALL})) return val_c_;
    return pc_;
}

uint8_t Fetch::GetICode() {
    if (mem_error_) return INOP;
    if (instruction_.empty()) {
        // Fetch::PrintErrorMessage(1);
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

uint64_t Fetch::GetValC(uint64_t pos) {
    uint64_t val_c = 0;
    // Read 8 bytes from instruction
    for (size_t i = 0; i < 8; ++i) val_c += instruction_[pos + i] << (i << 3);
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

bool Fetch::NeedBubble() { return false; }

bool Fetch::NeedStall() {
    // Conditions for a load / use hazard
    auto e_icode = PipelineRegister::Get(EXECUTE, assets::I_CODE);
    auto e_dst_m = PipelineRegister::Get(EXECUTE, assets::DST_M);
    auto d_src_a = Decode::src_a();
    auto d_src_b = Decode::src_b();
    if (ValueIsInArray(e_icode, {IMRMOVQ, IPOPQ}) &&
        ValueIsInArray(e_dst_m, {d_src_a, d_src_b}))
        return true;
    // Stalling at fetch while ret passes through pipeline
    auto d_icode = PipelineRegister::Get(DECODE, assets::I_CODE);
    auto m_icode = PipelineRegister::Get(MEMORY, assets::I_CODE);
    if (ValueIsInArray(static_cast<uint64_t>(IRET),
                       {d_icode, e_icode, m_icode}))
        return true;
    return false;
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
