#include "fetch.h"

#include <iostream>
#include <utility>

#include "../assets/file.h"
#include "../assets/register.h"
#include "../utils/utility.h"
#include "instruction.h"

using assets::File;
using assets::PipelineRegister;
using assets::ProgramCounter;

namespace stages {

bool Fetch::Do(const File& input) {
    auto pc        = ProgramCounter::Get();
    bool mem_error = false;
    instruction_   = input.GetInstruction(pc, &mem_error);
    auto icode     = GetICode(&mem_error);
    bool ins_valid = true;
    if (!InstructionIsValid(icode)) {
        Fetch::PrintErrorMessage(2);
        ins_valid = false;
    }
    auto ifun = GetIFun();

    if (mem_error)
        PipelineRegister::Set(assets::FETCH, assets::STAT, assets::SADR);
    else if (icode == IHALT)
        PipelineRegister::Set(assets::FETCH, assets::STAT, assets::SHLT);
    else if (!ins_valid)
        PipelineRegister::Set(assets::FETCH, assets::STAT, assets::SINS);
    else
        PipelineRegister::Set(assets::FETCH, assets::STAT, assets::SAOK);

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
    std::vector<uint8_t> valid_icodes{IHALT,   INOP,    IRRMOVQ, IIRMOVQ,
                                      IRMMOVQ, IMRMOVQ, IOPQ,    IJXX,
                                      ICALL,   IRET,    IPUSHQ,  IPOPQ};
    return utility::ValueIsInArray(icode, std::move(valid_icodes));
}

bool Fetch::NeedRegids(uint8_t icode) {
    std::vector<uint8_t> valid_icodes{IRRMOVQ, IOPQ,    IPUSHQ, IPOPQ,
                                      IIRMOVQ, IRMMOVQ, IMRMOVQ};
    return utility::ValueIsInArray(icode, std::move(valid_icodes));
}

bool Fetch::NeedValC(uint8_t icode) {
    std::vector<uint8_t> valid_icodes{IIRMOVQ, IRMMOVQ, IMRMOVQ, IJXX, ICALL};
    return utility::ValueIsInArray(icode, std::move(valid_icodes));
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
