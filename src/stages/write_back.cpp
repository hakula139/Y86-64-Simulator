#include "write_back.h"

#include <iostream>

#include "../assets/register.h"
#include "../utils/utility.h"
#include "instruction.h"

using assets::PipelineRegister;
using assets::Register;
using utility::ValueIsInArray;

using assets::DECODE;
using assets::EXECUTE;
using assets::FETCH;
using assets::MEMORY;
using assets::WRITE_BACK;

namespace stages {

uint8_t WriteBack::stat_;
bool    WriteBack::bubble_ = false;
bool    WriteBack::stall_  = false;

uint8_t WriteBack::Do() {
    bubble_ = NeedBubble();
    stall_  = NeedStall();

    stat_      = PipelineRegister::Get(WRITE_BACK, assets::STAT);
    auto val_e = PipelineRegister::Get(WRITE_BACK, assets::VAL_E);
    auto val_m = PipelineRegister::Get(WRITE_BACK, assets::VAL_M);
    auto dst_e = PipelineRegister::Get(WRITE_BACK, assets::VAL_E);
    auto dst_m = PipelineRegister::Get(WRITE_BACK, assets::VAL_M);

    Register::Set(dst_e, val_e);
    Register::Set(dst_m, val_m);

    return GetStat();
}

uint8_t WriteBack::GetStat() {
    if (bubble_) return assets::SAOK;
    return stat_;
}

bool WriteBack::NeedBubble() { return false; }

bool WriteBack::NeedStall() {
    auto w_stat = PipelineRegister::Get(WRITE_BACK, assets::STAT);
    if (ValueIsInArray(w_stat, {assets::SADR, assets::SINS, assets::SHLT}))
        return true;
    return false;
}

bool WriteBack::PrintErrorMessage(const int error_code) {
    std::cerr << "Write Back Error ";
    switch (error_code) {
        default: std::cerr << "X: An unknown error occurs.\n"; break;
    }
    return true;
}

}  // namespace stages
