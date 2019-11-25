#include "register.h"

namespace assets {

std::vector<uint64_t> Register::data_(kTotal_);

bool Register::Set(int register_num, uint64_t value) {
    data_.at(register_num) = value;
    return true;
}

bool Register::Clear() {
    for (auto&& value : data_) value = 0;
    return true;
}

}  // namespace assets
