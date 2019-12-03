#include "execute.h"

#include <iostream>
#include <vector>

#include "../assets/alu.h"
#include "../assets/register.h"
#include "../utils/utility.h"
#include "decode.h"
#include "instruction.h"
#include "memory.h"

using assets::ArithmeticLogicUnit;
using assets::ConditionCode;
using assets::PipelineRegister;
using utility::ValueIsInArray;

using assets::DECODE;
using assets::EXECUTE;
using assets::FETCH;
using assets::MEMORY;
using assets::WRITE_BACK;

namespace stages {

uint64_t Execute::val_a_;
uint64_t Execute::val_e_;
uint64_t Execute::dst_e_;
uint64_t Execute::dst_m_;
uint8_t  Execute::icode_;
uint8_t  Execute::ifun_;
uint8_t  Execute::stat_;
bool     Execute::cnd_;

bool Execute::Do() {
    stat_         = PipelineRegister::Get(EXECUTE, assets::STAT);
    icode_        = PipelineRegister::Get(EXECUTE, assets::I_CODE);
    ifun_         = PipelineRegister::Get(EXECUTE, assets::I_FUN);
    auto alu_a    = GetAluA();
    auto alu_b    = GetAluB();
    auto alu_func = GetAluFunction();
    cnd_          = GetCondition();
    val_e_        = GetValE(alu_a, alu_b, alu_func);
    val_a_        = PipelineRegister::Get(EXECUTE, assets::VAL_A);
    dst_e_        = GetDstE();
    dst_m_        = PipelineRegister::Get(EXECUTE, assets::DST_M);
    return true;
}

uint64_t Execute::GetAluA() {
    if (ValueIsInArray(icode_, {IRRMOVQ, IOPQ}))
        return PipelineRegister::Get(EXECUTE, assets::VAL_A);
    if (ValueIsInArray(icode_, {IIRMOVQ, IRMMOVQ, IMRMOVQ}))
        return PipelineRegister::Get(EXECUTE, assets::VAL_C);
    if (ValueIsInArray(icode_, {ICALL, IPUSHQ})) return -8ull;
    if (ValueIsInArray(icode_, {IRET, IPOPQ})) return 8ull;
    return 0;  // Other instructions don’t need ALU
}

uint64_t Execute::GetAluB() {
    if (ValueIsInArray(icode_,
                       {IRMMOVQ, IMRMOVQ, IOPQ, ICALL, IPUSHQ, IRET, IPOPQ}))
        return PipelineRegister::Get(EXECUTE, assets::VAL_B);
    if (ValueIsInArray(icode_, {IRRMOVQ, IIRMOVQ})) return 0ull;
    return 0;  // Other instructions don’t need ALU
}

uint64_t Execute::GetAluFunction() {
    if (icode_ == IOPQ) return PipelineRegister::Get(EXECUTE, assets::I_FUN);
    return ALUADD;
}

bool Execute::GetCondition() {
    auto zf     = ConditionCode::Get(assets::ZF);
    auto sf     = ConditionCode::Get(assets::SF);
    auto of     = ConditionCode::Get(assets::OF);
    bool result = true;
    switch (ifun_) {
        case assets::C_LE: result = zf || (sf && !of) || (!sf && of); break;
        case assets::C_L: result = (sf && !of) || (!sf && of); break;
        case assets::C_E: result = zf; break;
        case assets::C_NE: result = !zf; break;
        case assets::C_GE: result = (sf && of) || (!sf && !of); break;
        case assets::C_G: result = !zf && ((sf && of) || (!sf && !of)); break;
        default: break;
    }
    return result;
}

uint64_t Execute::GetValE(uint64_t alu_a, uint64_t alu_b, uint64_t alu_func) {
    if (icode_ == IJXX) return val_e_;
    auto    num_a  = static_cast<int64_t>(alu_a);
    auto    num_b  = static_cast<int64_t>(alu_b);
    int64_t result = 0;
    switch (alu_func) {
        case assets::ADDQ:
            result = ArithmeticLogicUnit::Add(num_a, num_b);
            break;
        case assets::SUBQ:
            result = ArithmeticLogicUnit::Sub(num_a, num_b);
            break;
        case assets::ANDQ:
            result = ArithmeticLogicUnit::And(num_a, num_b);
            break;
        case assets::XORQ:
            result = ArithmeticLogicUnit::Xor(num_a, num_b);
            break;
        default: break;
    }
    return static_cast<uint64_t>(result);
}

uint64_t Execute::GetDstE() {
    // Set dstE to RNONE in event of not-taken conditional move
    auto icode_ = PipelineRegister::Get(EXECUTE, assets::I_CODE);
    if (icode_ == IRRMOVQ && !cnd_) return assets::RNONE;
    return PipelineRegister::Get(EXECUTE, assets::DST_E);
}

bool Execute::NeedUpdateCC() {
    if (icode_ != IOPQ) return false;
    // State changes only during normal operation
    std::vector<uint8_t> mem_error_status{assets::SADR, assets::SINS,
                                          assets::SHLT};
    if (ValueIsInArray(Memory::stat(), mem_error_status)) return false;
    auto w_stat = PipelineRegister::Get(WRITE_BACK, assets::STAT);
    if (ValueIsInArray(static_cast<uint8_t>(w_stat), mem_error_status))
        return false;
    return true;
}

bool Execute::NeedBubble() {
    // Mispredicted branch
    auto e_icode = PipelineRegister::Get(EXECUTE, assets::I_CODE);
    auto e_cnd   = Execute::cnd();
    if (e_icode == IJXX && !e_cnd) return true;
    // Conditions for a load/use hazard
    auto e_dst_m = PipelineRegister::Get(EXECUTE, assets::DST_M);
    auto d_src_a = Decode::src_a();
    auto d_src_b = Decode::src_b();
    if (ValueIsInArray(e_icode, {IMRMOVQ, IPOPQ}) &&
        ValueIsInArray(e_dst_m, {d_src_a, d_src_b}))
        return true;
    return false;
}

bool Execute::NeedStall() { return false; }

bool Execute::PrintErrorMessage(const int error_code) {
    std::cerr << "Execute Error ";
    switch (error_code) {
        default: std::cerr << "X: An unknown error occurs.\n"; break;
    }
    return true;
}

}  // namespace stages
