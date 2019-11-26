#include "register.h"

#include <iostream>
#include <string>
#include <vector>

#include "../utils/utility.h"

using std::string;
using std::vector;

namespace assets {

uint64_t                 ProgramCounter::current_address_ = 0ull;
vector<uint64_t>         Register::data_(kTotal_);
vector<vector<uint64_t>> PipelineRegister::data_(kStageCount_,
                                                 vector<uint64_t>(kTotal_));
vector<uint64_t>         ConditionCode::data_(kTotal_);

bool ProgramCounter::Set(uint64_t value) {
    current_address_ = value;
    return true;
}

bool ProgramCounter::Clear() {
    current_address_ = 0ull;
    return true;
}

bool ProgramCounter::Print() {
    std::cout << "PC = " << Get() << '\n';
    return true;
}

bool Register::Set(int register_num, uint64_t value) {
    data_.at(register_num) = value;
    return true;
}

bool Register::Clear() {
    for (auto&& value : data_) value = 0;
    return true;
}

bool Register::Print() {
    vector<string> register_name{"RAX", "RCX", "RDX", "RBX", "RSP",
                                 "RBP", "RSI", "RDI", "R8 ", "R9 ",
                                 "R10", "R11", "R12", "R13", "R14"};
    for (size_t i = 0; i < kTotal_ - 1; ++i) {
        std::cout << '%' << register_name[i] << " = 0x";
        utility::SetOutputHexWidth(8);
        std::cout << Get(i) << '\n';
    }
    return true;
}

bool PipelineRegister::Set(int stage_num, int register_num, uint64_t value) {
    data_.at(stage_num).at(register_num) = value;
    return true;
}

bool PipelineRegister::Clear(int stage_num) {
    for (auto&& value : data_[stage_num]) value = 0;
    return true;
}

bool PipelineRegister::Print(int stage_num) {
    vector<string> stage_name{"FETCH", "DECODE", "EXECUTE", "MEMORY",
                              "WRITE BACK"};

    vector<vector<string>> stage_register_name{
        {"PRED_PC"},  // Fetch Stage
        {"STAT", "I_CODE", "I_FUN", "R_A", "R_B", "VAL_C",
         "VAL_P"},  // Decode Stage
        {"STAT", "I_CODE", "I_FUN", "VAL_C", "VAL_A", "VAL_B", "DST_E", "DST_M",
         "SRC_A", "SRC_B"},  // Execute Stage
        {"STAT", "I_CODE", "CND", "VAL_E", "VAL_A", "DST_E",
         "DST_M"},  // Memory Stage
        {"STAT", "I_CODE", "VAL_E", "VAL_M", "DST_E",
         "DST_M"}  // Write Back Stage
    };

    vector<vector<int>> stage_register_num{
        {PRED_PC},                                      // Fetch Stage
        {STAT, I_CODE, I_FUN, R_A, R_B, VAL_C, VAL_P},  // Decode Stage
        {STAT, I_CODE, I_FUN, VAL_C, VAL_A, VAL_B, DST_E, DST_M, SRC_A,
         SRC_B},                                          // Execute Stage
        {STAT, I_CODE, CND, VAL_E, VAL_A, DST_E, DST_M},  // Memory Stage
        {STAT, I_CODE, VAL_E, VAL_M, DST_E, DST_M}        // Write Back Stage
    };

    std::cout << string(5, '=') << ' ' << stage_name[stage_num] << ' '
              << string(5, '=') << '\n';
    for (auto&& i : stage_register_num[stage_num]) {
        std::cout << stage_register_name[stage_num][i] << "\t = 0x";
        utility::SetOutputHexWidth(8);
        std::cout << Get(stage_num, i) << '\n';
    }
    return true;
}

bool ConditionCode::Print() {
    vector<string> register_name{"OF", "SF", "ZF", "CF"};
    for (size_t i = 0; i < kTotal_; ++i) {
        std::cout << register_name[i] << Get(i) << '\n';
    }
    return true;
}

}  // namespace assets
