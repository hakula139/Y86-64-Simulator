#include "execute.h"

#include <iostream>
#include <utility>

#include "../assets/alu.h"
#include "../assets/register.h"
#include "../utils/utility.h"
#include "instruction.h"

using std::move;
using std::vector;

using assets::ArithmeticLogicUnit;
using assets::PipelineRegister;
using utility::ValueIsInArray;

using assets::EXECUTE;
using assets::MEMORY;

namespace stages {

bool Execute::Do() {
    auto stat     = PipelineRegister::Get(EXECUTE, assets::STAT);
    auto icode    = PipelineRegister::Get(EXECUTE, assets::I_CODE);
    auto ifun     = PipelineRegister::Get(EXECUTE, assets::I_FUN);
    auto alu_a    = GetAluA(icode);
    auto alu_b    = GetAluB(icode);
    auto alu_func = GetAluFunction(icode);
    auto cnd      = GetCondition(ifun);
    auto val_e    = GetValE(alu_a, alu_b, alu_func);
    auto val_a    = PipelineRegister::Get(EXECUTE, assets::VAL_A);
    auto dst_e    = PipelineRegister::Get(EXECUTE, assets::DST_E);
    auto dst_m    = PipelineRegister::Get(EXECUTE, assets::DST_M);

    PipelineRegister::Set(MEMORY, assets::STAT, stat);
    PipelineRegister::Set(MEMORY, assets::I_CODE, icode);
    PipelineRegister::Set(MEMORY, assets::CND, cnd);
    PipelineRegister::Set(MEMORY, assets::VAL_E, val_e);
    PipelineRegister::Set(MEMORY, assets::VAL_A, val_a);
    if (cnd) PipelineRegister::Set(MEMORY, assets::DST_E, dst_e);
    PipelineRegister::Set(MEMORY, assets::DST_M, dst_m);

    return true;
}

uint64_t Execute::GetAluA(uint8_t icode) {
    if (ValueIsInArray(icode, {IRRMOVQ, IOPQ}))
        return PipelineRegister::Get(EXECUTE, assets::VAL_A);
    if (ValueIsInArray(icode, {IIRMOVQ, IRMMOVQ, IMRMOVQ}))
        return PipelineRegister::Get(EXECUTE, assets::VAL_C);
    if (ValueIsInArray(icode, {ICALL, IPUSHQ})) return -8ull;
    if (ValueIsInArray(icode, {IRET, IPOPQ})) return 8ull;
    return 0;  // Other instructions don’t need ALU
}

uint64_t Execute::GetAluB(uint8_t icode) {
    if (ValueIsInArray(icode,
                       {IRMMOVQ, IMRMOVQ, IOPQ, ICALL, IPUSHQ, IRET, IPOPQ}))
        return PipelineRegister::Get(EXECUTE, assets::VAL_B);
    if (ValueIsInArray(icode, {IRRMOVQ, IIRMOVQ})) return 0ull;
    return 0;  // Other instructions don’t need ALU
}

uint64_t Execute::GetAluFunction(uint8_t icode) {
    if (icode == IOPQ) return PipelineRegister::Get(EXECUTE, assets::I_FUN);
    return ALUADD;
}

bool Execute::GetCondition(uint8_t ifun) {
    auto zf     = assets::ConditionCode::Get(assets::ZF);
    auto sf     = assets::ConditionCode::Get(assets::SF);
    auto of     = assets::ConditionCode::Get(assets::OF);
    bool result = 1;
    switch (ifun) {
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

bool Execute::NeedUpdateCC(uint8_t icode) { return icode == IOPQ; }

bool Execute::PrintErrorMessage(const int error_code) {
    std::cerr << "Execute Error ";
    switch (error_code) {
        default: std::cerr << "X: An unknown error occurs.\n"; break;
    }
    return true;
}

}  // namespace stages
