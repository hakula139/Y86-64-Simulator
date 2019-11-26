#include "write_back.h"

#include <iostream>

#include "../assets/register.h"
#include "instruction.h"

using assets::PipelineRegister;

using assets::DECODE;
using assets::EXECUTE;
using assets::FETCH;
using assets::MEMORY;
using assets::WRITE_BACK;

namespace stages {

bool WriteBack::Do() {
    auto stat = PipelineRegister::Get(WRITE_BACK, assets::STAT);
    if (stat == assets::SBUB)
        PipelineRegister::Set(MEMORY, assets::STAT, assets::SAOK);
    else
        PipelineRegister::Set(WRITE_BACK, assets::STAT, stat);
    return stat;
}

bool WriteBack::PrintErrorMessage(const int error_code) {
    std::cerr << "Write Back Error ";
    switch (error_code) {
        default: std::cerr << "X: An unknown error occurs.\n"; break;
    }
    return true;
}

}  // namespace stages
