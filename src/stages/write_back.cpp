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

uint8_t  WriteBack::stat_;
uint64_t WriteBack::val_e_;
uint64_t WriteBack::dst_m_;
uint64_t WriteBack::val_m_;

uint8_t WriteBack::Do() {
    stat_  = PipelineRegister::Get(WRITE_BACK, assets::STAT);
    val_e_ = PipelineRegister::Get(WRITE_BACK, assets::VAL_E);
    return GetStat();
}

uint8_t WriteBack::GetStat() {
    if (stat() == assets::SBUB) return assets::SAOK;
    return stat();
}

bool WriteBack::PrintErrorMessage(const int error_code) {
    std::cerr << "Write Back Error ";
    switch (error_code) {
        default: std::cerr << "X: An unknown error occurs.\n"; break;
    }
    return true;
}

}  // namespace stages
