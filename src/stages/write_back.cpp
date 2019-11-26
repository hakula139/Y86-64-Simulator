#include "write_back.h"

#include <iostream>
#include <utility>

#include "../assets/alu.h"
#include "../assets/register.h"
#include "../utils/utility.h"
#include "instruction.h"

using std::move;
using std::vector;

using assets::ArithmeticLogicUnit;
using assets::ConditionCode;
using assets::PipelineRegister;
using utility::ValueIsInArray;

using assets::DECODE;
using assets::EXECUTE;
using assets::FETCH;
using assets::MEMORY;
using assets::WRITE_BACK;

namespace stage {

bool WriteBack::Do() {
    auto stat = PipelineRegister::Get(WRITE_BACK, assets::STAT);
    if (stat == SBUB)
        PipelineRegister::Set(MEMORY, assets::STAT, SAOK);
    else
        PipelineRegister::Set(WRITE_BACK, assets::STAT, stat);
}

bool WriteBack::PrintErrorMessage(const int error_code) {
    std::cerr << "Execute Error ";
    switch (error_code) {
        default: std::cerr << "X: An unknown error occurs.\n"; break;
    }
    return true;
}

}  // namespace stage