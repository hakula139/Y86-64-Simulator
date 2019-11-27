#include "decode.h"

#include <iostream>

#include "../assets/register.h"
#include "../utils/utility.h"
#include "execute.h"
#include "instruction.h"
#include "memory.h"

using assets::PipelineRegister;
using assets::Register;
using utility::ValueIsInArray;

using assets::DECODE;
using assets::EXECUTE;
using assets::FETCH;
using assets::MEMORY;
using assets::WRITE_BACK;

namespace stages {

uint64_t Decode::dst_e_;
uint64_t Decode::dst_m_;
uint64_t Decode::src_a_;
uint64_t Decode::src_b_;
bool     Decode::bubble_ = false;
bool     Decode::stall_  = false;

bool Decode::Do() {
    bubble_ = NeedBubble();
    stall_  = NeedStall();
    if (!Execute::bubble() && Execute::stall()) return false;

    auto stat  = PipelineRegister::Get(DECODE, assets::STAT);
    auto icode = PipelineRegister::Get(DECODE, assets::I_CODE);
    auto ifun  = PipelineRegister::Get(DECODE, assets::I_FUN);
    auto val_c = PipelineRegister::Get(DECODE, assets::VAL_C);
    auto val_a = GetValA(icode);
    auto val_b = GetValB(icode);
    dst_e_     = GetDstE(icode);
    dst_m_     = GetDstM(icode);
    src_a_     = GetSrcA(icode);
    src_b_     = GetSrcB(icode);

    PipelineRegister::Set(EXECUTE, assets::STAT, stat);
    PipelineRegister::Set(EXECUTE, assets::I_CODE, icode);
    PipelineRegister::Set(EXECUTE, assets::I_FUN, ifun);
    PipelineRegister::Set(EXECUTE, assets::VAL_C, val_c);
    PipelineRegister::Set(EXECUTE, assets::VAL_A, val_a);
    PipelineRegister::Set(EXECUTE, assets::VAL_B, val_b);
    PipelineRegister::Set(EXECUTE, assets::DST_E, dst_e_);
    PipelineRegister::Set(EXECUTE, assets::DST_M, dst_m_);
    PipelineRegister::Set(EXECUTE, assets::SRC_A, src_a_);
    PipelineRegister::Set(EXECUTE, assets::SRC_B, src_b_);

    if (Execute::bubble()) PipelineRegister::Clear(EXECUTE);
    return true;
}

uint64_t Decode::GetValA(uint8_t icode) {
    // Uses incremented PC
    if (ValueIsInArray(icode, {ICALL, IJXX}))
        return PipelineRegister::Get(DECODE, assets::VAL_P);
    // Forwards valE from execute
    if (src_a_ == Execute::dst_e()) return Execute::val_e();
    // Forwards valM from memory
    if (src_a_ == PipelineRegister::Get(MEMORY, assets::DST_M))
        return Memory::val_m();
    // Forwards valE from memory
    if (src_a_ == PipelineRegister::Get(MEMORY, assets::DST_E))
        return PipelineRegister::Get(MEMORY, assets::VAL_E);
    // Forwards valM from write back
    if (src_a_ == PipelineRegister::Get(WRITE_BACK, assets::DST_M))
        return PipelineRegister::Get(WRITE_BACK, assets::VAL_M);
    // Forwards valE from write back
    if (src_a_ == PipelineRegister::Get(WRITE_BACK, assets::DST_E))
        return PipelineRegister::Get(WRITE_BACK, assets::VAL_E);
    // Uses value read from register file
    return Register::Get(assets::VAL_A);
}

uint64_t Decode::GetValB(uint8_t icode) {
    // Forwards valE from execute
    if (src_b_ == Execute::dst_e()) return Execute::val_e();
    // Forwards valM from memory
    if (src_b_ == PipelineRegister::Get(MEMORY, assets::DST_M))
        return Memory::val_m();
    // Forwards valE from memory
    if (src_b_ == PipelineRegister::Get(MEMORY, assets::DST_E))
        return PipelineRegister::Get(MEMORY, assets::VAL_E);
    // Forwards valM from write back
    if (src_b_ == PipelineRegister::Get(WRITE_BACK, assets::DST_M))
        return PipelineRegister::Get(WRITE_BACK, assets::VAL_M);
    // Forwards valE from write back
    if (src_b_ == PipelineRegister::Get(WRITE_BACK, assets::DST_E))
        return PipelineRegister::Get(WRITE_BACK, assets::VAL_E);
    // Uses value read from register file
    return Register::Get(assets::VAL_B);
}

uint64_t Decode::GetSrcA(uint8_t icode) {
    if (ValueIsInArray(icode, {IRRMOVQ, IRMMOVQ, IOPQ, IPUSHQ}))
        return PipelineRegister::Get(DECODE, assets::R_A);
    if (ValueIsInArray(icode, {IPOPQ, IRET})) return assets::RSP;
    return assets::RNONE;  // Don’t need register
}

uint64_t Decode::GetSrcB(uint8_t icode) {
    if (ValueIsInArray(icode, {IMRMOVQ, IRMMOVQ, IOPQ}))
        return PipelineRegister::Get(DECODE, assets::R_B);
    if (ValueIsInArray(icode, {IPUSHQ, IPOPQ, ICALL, IRET})) return assets::RSP;
    return assets::RNONE;  // Don’t need register
}

uint64_t Decode::GetDstE(uint8_t icode) {
    if (ValueIsInArray(icode, {IRRMOVQ, IIRMOVQ, IOPQ}))
        return PipelineRegister::Get(DECODE, assets::R_B);
    if (ValueIsInArray(icode, {IPUSHQ, IPOPQ, ICALL, IRET})) return assets::RSP;
    return assets::RNONE;  // Don’t write any register
}

uint64_t Decode::GetDstM(uint8_t icode) {
    if (ValueIsInArray(icode, {IMRMOVQ, IOPQ}))
        return PipelineRegister::Get(DECODE, assets::R_A);
    return assets::RNONE;  // Don’t write any register
}

bool Decode::NeedBubble() {
    // Mispredicted branch
    auto e_icode = PipelineRegister::Get(EXECUTE, assets::I_CODE);
    auto e_cnd   = PipelineRegister::Get(EXECUTE, assets::CND);
    if (e_icode == IJXX && !e_cnd) return true;
    // Stalling at fetch while ret passes through pipeline but not condition for
    // a load / use hazard
    auto e_dst_m = PipelineRegister::Get(EXECUTE, assets::DST_M);
    auto d_src_a = PipelineRegister::Get(DECODE, assets::SRC_A);
    auto d_src_b = PipelineRegister::Get(DECODE, assets::SRC_B);
    auto d_icode = PipelineRegister::Get(DECODE, assets::I_CODE);
    auto m_icode = PipelineRegister::Get(MEMORY, assets::I_CODE);
    if (!ValueIsInArray(e_icode, {IMRMOVQ, IPOPQ}) &&
        ValueIsInArray(e_dst_m, {d_src_a, d_src_b}) &&
        ValueIsInArray(static_cast<uint64_t>(IRET),
                       {d_icode, e_icode, m_icode}))
        return true;
    return false;
}

bool Decode::NeedStall() {
    // Conditions for a load / use hazard
    auto e_icode = PipelineRegister::Get(EXECUTE, assets::I_CODE);
    auto e_dst_m = PipelineRegister::Get(EXECUTE, assets::DST_M);
    auto d_src_a = PipelineRegister::Get(DECODE, assets::SRC_A);
    auto d_src_b = PipelineRegister::Get(DECODE, assets::SRC_B);
    if (ValueIsInArray(e_icode, {IMRMOVQ, IPOPQ}) &&
        ValueIsInArray(e_dst_m, {d_src_a, d_src_b}))
        return true;
}

bool Decode::PrintErrorMessage(const int error_code) {
    std::cerr << "Decode Error ";
    switch (error_code) {
        default: std::cerr << "X: An unknown error occurs.\n"; break;
    }
    return true;
}

}  // namespace stages
