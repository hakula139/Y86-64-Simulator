#include "register.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "../stages/instruction.h"
#include "../utils/utility.h"
#include "cpu_clock.h"
#include "json.hpp"

using nlohmann::json;
using std::map;
using std::move;
using std::ofstream;
using std::string;
using std::vector;

namespace assets {

vector<uint64_t>         Register::data_(kTotal_);
vector<vector<uint64_t>> PipelineRegister::data_(kStageCount_,
                                                 vector<uint64_t>(kTotal_));
vector<bool>             ConditionCode::data_(kTotal_);

map<uint64_t, vector<Change>>         Register::changes_;
vector<map<uint64_t, vector<Change>>> PipelineRegister::changes_(kStageCount_);
map<uint64_t, vector<Change>>         ConditionCode::changes_;

static const vector<string> register_name{"RAX", "RCX", "RDX", "RBX", "RSP",
                                          "RBP", "RSI", "RDI", "R8 ", "R9 ",
                                          "R10", "R11", "R12", "R13", "R14"};

static const vector<int> all_register_num{
    RAX, RCX, RDX, RBX, RSP, RBP, RSI, RDI, R8, R9, R10, R11, R12, R13, R14};

static const vector<string> stage_name{"FETCH", "DECODE", "EXECUTE", "MEMORY",
                                       "WRITEBACK"};

static const vector<string> stage_register_name{
    "PRED_PC", "STAT",  "I_CODE", "I_FUN", "R_A",   "R_B",
    "VAL_C",   "VAL_P", "VAL_A",  "VAL_B", "VAL_E", "VAL_M",
    "SRC_A",   "SRC_B", "DST_E",  "DST_M", "CND"};

static const vector<int> all_stage_register_num{
    PRED_PC, STAT,  I_CODE, I_FUN, R_A,   R_B,   VAL_C, VAL_P, VAL_A,
    VAL_B,   VAL_E, VAL_M,  SRC_A, SRC_B, DST_E, DST_M, CND};

static const vector<uint64_t> all_stage_register_init{
    0, SBUB, stages::INOP, 0,     RNONE, RNONE, 0, 0, 0, 0,
    0, 0,    RNONE,        RNONE, RNONE, RNONE, 0};

static const vector<vector<int>> stage_register_num{
    {PRED_PC},                                      // Fetch Stage
    {STAT, I_CODE, I_FUN, R_A, R_B, VAL_C, VAL_P},  // Decode Stage
    {STAT, I_CODE, I_FUN, VAL_C, VAL_A, VAL_B, DST_E, DST_M, SRC_A,
     SRC_B},                                          // Execute Stage
    {STAT, I_CODE, CND, VAL_E, VAL_A, DST_E, DST_M},  // Memory Stage
    {STAT, I_CODE, VAL_E, VAL_M, DST_E, DST_M}        // Write Back Stage
};

static const vector<string> condition_code_name{"OF", "SF", "ZF"};

static const vector<int> all_condition_code_num{OF, SF, ZF};

static const char* output_path = "json/";

bool ChangesHandler::Set(const Change& change, int mode, int stage_num) {
    switch (mode) {
        case REG: Register::changes_[cpu_clock].push_back(change); break;
        case PIP:
            PipelineRegister::changes_.at(stage_num)[cpu_clock].push_back(
                change);
            break;
        case CC: ConditionCode::changes_[cpu_clock].push_back(change); break;
        default: return false;
    }
    return true;
}

json ChangesHandler::GetAllInJson(
    const map<uint64_t, vector<Change>>& all_changes) {
    json all_changes_json;
    for (auto&& clock_changes : all_changes) {
        json changes_json;
        for (auto&& change : clock_changes.second) {
            auto reg_name          = register_name[change.register_num];
            auto old_value         = change.old_value;
            auto new_value         = change.new_value;
            changes_json[reg_name] = {{"old", old_value}, {"new", new_value}};
        }
        auto clock              = clock_changes.first;
        all_changes_json[clock] = move(changes_json);
    }
    return move(all_changes_json);
}

bool ChangesHandler::PrintAllInJson() {
    bool status = true;
    status &= PrintRegister();
    status &= PrintPipelineRegister();
    status &= PrintConditionCode();
    return status;
}

bool ChangesHandler::PrintRegister() {
    ofstream output;
    auto     file_name = string(output_path) + "registerChanges.json";
    output.open(file_name, ofstream::out | ofstream::trunc);
    if (!output) return false;
    output << std::setw(4) << GetAllInJson(Register::changes_);
    output.close();
    return true;
}

bool ChangesHandler::PrintPipelineRegister() {
    ofstream output;
    for (auto&& stage_num : {FETCH, DECODE, EXECUTE, MEMORY, WRITE_BACK}) {
        auto file_name = stage_name[stage_num];
        for (auto&& c : file_name) c = std::tolower(c);
        file_name = output_path + file_name + "Changes.json";
        output.open(file_name, ofstream::out | ofstream::trunc);
        if (!output) return false;
        output << std::setw(4)
               << GetAllInJson(PipelineRegister::changes_[stage_num]);
        output.close();
    }
    return true;
}

bool ChangesHandler::PrintConditionCode() {
    ofstream output;
    auto     file_name = string(output_path) + "conditionCodeChanges.json";
    output.open(file_name, ofstream::out | ofstream::trunc);
    if (!output) return false;
    output << std::setw(4) << GetAllInJson(ConditionCode::changes_);
    output.close();
    return true;
}

bool Register::Set(int register_num, uint64_t value) {
    auto old_value = data_.at(register_num);
    if (value != old_value) {
        Change change{register_num, old_value, value};
        ChangesHandler::Set(move(change), REG);
        data_[register_num] = value;
    }
    return true;
}

bool Register::Clear() {
    for (auto&& register_num : all_register_num) {
        auto old_value = data_.at(register_num);
        if (old_value) {
            Change change{register_num, old_value, 0ull};
            ChangesHandler::Set(move(change), REG);
            data_[register_num] = 0ull;
        }
    }
    return true;
}

bool Register::Print() {
    std::cout << string(5, '=') << " REGISTER " << string(5, '=') << '\n';
    for (auto&& i : all_register_num) {
        std::cout << '%' << register_name[i] << " = 0x";
        utility::SetOutputHexWidth(16);
        std::cout << Get(i) << '\n';
    }
    return true;
}

bool PipelineRegister::Set(int stage_num, int register_num, uint64_t value) {
    auto old_value = data_.at(stage_num).at(register_num);
    if (value != old_value) {
        Change change{register_num, old_value, value};
        ChangesHandler::Set(move(change), PIP, stage_num);
        data_[stage_num][register_num] = value;
    }
    return true;
}

bool PipelineRegister::Clear(int stage_num) {
    auto& stage_data = data_.at(stage_num);
    for (auto&& register_num : all_stage_register_num) {
        auto old_value = stage_data.at(register_num);
        auto new_value = all_stage_register_init.at(register_num);
        if (new_value != old_value) {
            Change change{register_num, old_value, new_value};
            ChangesHandler::Set(move(change), PIP, stage_num);
            stage_data[register_num] = new_value;
        }
    }
    return true;
}

bool PipelineRegister::Print(int stage_num) {
    std::cout << string(5, '=') << ' ' << stage_name[stage_num] << ' '
              << string(5, '=') << '\n';
    for (auto&& i : stage_register_num[stage_num]) {
        std::cout << stage_register_name[i] << "\t = 0x";
        utility::SetOutputHexWidth(16);
        std::cout << Get(stage_num, i) << '\n';
    }
    return true;
}

bool ConditionCode::Set(int register_num, bool value) {
    auto old_value = data_.at(register_num);
    if (value != old_value) {
        Change change{register_num, old_value, value};
        ChangesHandler::Set(move(change), CC);
        data_[register_num] = value;
    }
    return true;
}

bool ConditionCode::Clear() {
    for (auto&& register_num : all_condition_code_num) {
        auto old_value = data_.at(register_num);
        if (old_value) {
            Change change{register_num, old_value, 0};
            ChangesHandler::Set(move(change), REG);
            data_[register_num] = 0;
        }
    }
    return true;
}

bool ConditionCode::Print() {
    std::cout << string(5, '=') << " CONDITION CODE " << string(5, '=') << '\n';
    for (size_t i = 0; i < kTotal_; ++i) {
        std::cout << condition_code_name[i] << " = " << Get(i) << '\n';
    }
    return true;
}

}  // namespace assets
