#ifndef SRC_UTILS_UTILITY_H_
#define SRC_UTILS_UTILITY_H_

#include <cstddef>
#include <vector>

namespace utility {

bool SetOutputHexWidth(size_t width);

template <class T>
bool ValueIsInArray(T value, const std::vector<T>& array) {
    for (const auto& i : array)
        if (value == i) return true;
    return false;
}

template <class T>
bool ValueIsInArray(T value, std::vector<T>&& array) {
    for (const auto& i : array)
        if (value == i) return true;
    return false;
}

}  // namespace utility

#endif  // SRC_UTILS_UTILITY_H_
