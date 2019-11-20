#include "register.h"

namespace assets {

bool Register::Set(int reg, uint64_t value) {
    data_.at(reg) = value;
    return true;
}

bool Register::Clear() {
    for (auto&& value : data_) value = 0;
    return true;
}

bool ConditionCode::Set(int condition_code, uint64_t value) {
    data_.at(condition_code) = value;
    return true;
}

bool ConditionCode::Clear() {
    for (auto&& value : data_) value = 0;
    return true;
}

}  // namespace assets
