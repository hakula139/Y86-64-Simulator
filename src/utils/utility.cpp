#include "utility.h"

#include <iomanip>
#include <iostream>

namespace utility {

bool SetOutputHexWidth(const size_t width) {
    std::cout << std::hex << std::setfill('0') << std::setw(width);
    return true;
}

}  // namespace utility
