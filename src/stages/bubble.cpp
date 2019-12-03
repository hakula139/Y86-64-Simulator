#include "bubble.h"

#include <iostream>

#include "../assets/register.h"
#include "decode.h"
#include "execute.h"
#include "fetch.h"
#include "memory.h"
#include "write_back.h"

using assets::ConditionCode;
using assets::PipelineRegister;

using assets::DECODE;
using assets::EXECUTE;
using assets::FETCH;
using assets::MEMORY;
using assets::WRITE_BACK;

namespace stages {

bool Bubble::UpdateAll() {
    UpdateFetchStage();
    UpdateDecodeStage();
    UpdateExecuteStage();
    UpdateMemoryStage();
    UpdateWriteBackStage();
    return true;
}

bool Bubble::UpdateFetchStage() {
    PipelineRegister::Set(FETCH, assets::VAL_P, Fetch::pc_);
    if (Fetch::NeedBubble())
        PipelineRegister::Clear(FETCH);
    else if (!Fetch::NeedStall())
        PipelineRegister::Set(FETCH, assets::PRED_PC, Fetch::GetPredPC());
    return true;
}

bool Bubble::UpdateDecodeStage() {
    if (Decode::NeedBubble()) {
        PipelineRegister::Clear(DECODE);
    } else if (!Decode::NeedStall()) {
        PipelineRegister::Set(DECODE, assets::I_CODE, Fetch::icode_);
        PipelineRegister::Set(DECODE, assets::I_FUN, Fetch::ifun_);
        PipelineRegister::Set(DECODE, assets::STAT, Fetch::stat_);
        PipelineRegister::Set(DECODE, assets::VAL_P, Fetch::pc_);
        PipelineRegister::Set(DECODE, assets::R_A, Fetch::r_a_);
        PipelineRegister::Set(DECODE, assets::R_B, Fetch::r_b_);
        PipelineRegister::Set(DECODE, assets::VAL_C, Fetch::val_c_);
    }
    return true;
}

bool Bubble::UpdateExecuteStage() {
    if (Execute::NeedBubble()) {
        PipelineRegister::Clear(EXECUTE);
    } else if (!Execute::NeedStall()) {
        PipelineRegister::Set(EXECUTE, assets::STAT, Decode::stat_);
        PipelineRegister::Set(EXECUTE, assets::I_CODE, Decode::icode_);
        PipelineRegister::Set(EXECUTE, assets::I_FUN, Decode::ifun_);
        PipelineRegister::Set(EXECUTE, assets::VAL_C, Decode::val_c_);
        PipelineRegister::Set(EXECUTE, assets::VAL_A, Decode::val_a_);
        PipelineRegister::Set(EXECUTE, assets::VAL_B, Decode::val_b_);
        PipelineRegister::Set(EXECUTE, assets::DST_E, Decode::dst_e_);
        PipelineRegister::Set(EXECUTE, assets::DST_M, Decode::dst_m_);
        PipelineRegister::Set(EXECUTE, assets::SRC_A, Decode::src_a_);
        PipelineRegister::Set(EXECUTE, assets::SRC_B, Decode::src_b_);
    }
    return true;
}

bool Bubble::UpdateMemoryStage() {
    if (Memory::NeedBubble()) {
        PipelineRegister::Clear(MEMORY);
        if (Execute::NeedUpdateCC()) ConditionCode::Clear();
    } else if (!Memory::NeedStall()) {
        PipelineRegister::Set(MEMORY, assets::STAT, Execute::stat_);
        PipelineRegister::Set(MEMORY, assets::I_CODE, Execute::icode_);
        PipelineRegister::Set(MEMORY, assets::CND, Execute::cnd_);
        PipelineRegister::Set(MEMORY, assets::VAL_E, Execute::val_e_);
        PipelineRegister::Set(MEMORY, assets::VAL_A, Execute::val_a_);
        PipelineRegister::Set(MEMORY, assets::DST_E, Execute::dst_e_);
        PipelineRegister::Set(MEMORY, assets::DST_M, Execute::dst_m_);
    }
    return true;
}

bool Bubble::UpdateWriteBackStage() {
    if (WriteBack::NeedBubble()) {
        PipelineRegister::Clear(WRITE_BACK);
    } else if (!WriteBack::NeedStall()) {
        PipelineRegister::Set(WRITE_BACK, assets::STAT, Memory::stat_);
        PipelineRegister::Set(WRITE_BACK, assets::I_CODE, Memory::icode_);
        PipelineRegister::Set(WRITE_BACK, assets::VAL_E, Memory::val_e_);
        PipelineRegister::Set(WRITE_BACK, assets::VAL_M, Memory::val_m_);
        PipelineRegister::Set(WRITE_BACK, assets::DST_E, Memory::dst_e_);
        PipelineRegister::Set(WRITE_BACK, assets::DST_M, Memory::dst_m_);
    }
    return true;
}

}  // namespace stages
