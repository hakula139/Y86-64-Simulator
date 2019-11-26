#include "register.h"

#include <vector>

using std::vector;

namespace assets {

vector<uint64_t>         Register::data_(kTotal_);
vector<vector<uint64_t>> PipelineRegister::data_(kStageCount_,
                                                 vector<uint64_t>(kTotal_));
vector<uint64_t>         ConditionCode::data_(kTotal_);

bool ProgramCounter::Clear() {
    current_address_ = 0ull;
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

bool PipelineRegister::Set(int stage_num, int register_num, uint64_t value) {
    data_.at(stage_num).at(register_num) = value;
    return true;
}

bool PipelineRegister::Clear() {
    for (auto&& stage : data_)
        for (auto&& value : stage) value = 0;
    return true;
}

}  // namespace assets
