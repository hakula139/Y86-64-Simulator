#include "fetch.h"

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

namespace stages {

vector<uint8_t> Fetch::instruction_;

bool Fetch::Do(const File& input) {
    auto pc        = PipelineRegister::Get(assets::FETCH, assets::PRED_PC);
    bool mem_error = false;
    instruction_   = input.GetInstruction(pc, &mem_error);
    auto icode     = GetICode(&mem_error);
    bool ins_valid = true;
    if (!InstructionIsValid(icode)) {
        Fetch::PrintErrorMessage(2);
        ins_valid = false;
    }
    auto ifun = GetIFun();
    PipelineRegister::Set(DECODE, assets::I_CODE, icode);
    PipelineRegister::Set(DECODE, assets::I_FUN, ifun);
    ++pc;

    if (mem_error)
        PipelineRegister::Set(DECODE, assets::STAT, assets::SADR);
    else if (icode == IHALT)
        PipelineRegister::Set(DECODE, assets::STAT, assets::SHLT);
    else if (!ins_valid)
        PipelineRegister::Set(DECODE, assets::STAT, assets::SINS);
    else
        PipelineRegister::Set(DECODE, assets::STAT, assets::SAOK);

    if (NeedRegids(icode)) {
        PipelineRegister::Set(DECODE, assets::R_A, GetRA());
        PipelineRegister::Set(DECODE, assets::R_A, GetRB());
        ++pc;
    }

    if (NeedValC(icode)) {
        PipelineRegister::Set(DECODE, assets::VAL_C, GetValC());
        pc += 8;
    }

    PipelineRegister::Set(DECODE, assets::VAL_P, pc);
    return true;
}

uint8_t Fetch::GetICode(bool* mem_error) {
    if (instruction_.empty()) {
        Fetch::PrintErrorMessage(1);
        *mem_error = true;
        return INOP;
    }
    return (instruction_[0] >> 4) & 0xF;  // the first 4 bits
}

uint8_t Fetch::GetIFun() {
    if (instruction_.empty()) return FNONE;
    return instruction_[0] & 0xF;  // the second 4 bits
}

bool Fetch::InstructionIsValid(uint8_t icode) {
    vector<uint8_t> valid_icodes{IHALT,   INOP,    IRRMOVQ, IIRMOVQ,
                                 IRMMOVQ, IMRMOVQ, IOPQ,    IJXX,
                                 ICALL,   IRET,    IPUSHQ,  IPOPQ};
    return ValueIsInArray(icode, move(valid_icodes));
}

bool Fetch::NeedRegids(uint8_t icode) {
    vector<uint8_t> valid_icodes{IRRMOVQ, IOPQ,    IPUSHQ, IPOPQ,
                                 IIRMOVQ, IRMMOVQ, IMRMOVQ};
    return ValueIsInArray(icode, move(valid_icodes));
}

bool Fetch::NeedValC(uint8_t icode) {
    vector<uint8_t> valid_icodes{IIRMOVQ, IRMMOVQ, IMRMOVQ, IJXX, ICALL};
    return ValueIsInArray(icode, move(valid_icodes));
}

uint8_t Fetch::GetRA() { return (instruction_[1] >> 4) & 0xF; }

uint8_t Fetch::GetRB() { return instruction_[1] & 0xF; }

uint64_t Fetch::GetValC() {
    uint64_t val_c = 0;
    // Read 8 bytes from instruction
    for (size_t i = 2; i < 10; ++i) val_c = (val_c << 2) + instruction_[i];
    return val_c;
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
