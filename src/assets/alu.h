#ifndef SRC_ASSETS_ALU_H_
#define SRC_ASSETS_ALU_H_

#include <cstdint>

namespace assets {

class ArithmeticLogicUnit {
public:
    static int64_t  AddSigned(int64_t num1, int64_t num2);
    static uint64_t AddUnsigned(uint64_t num1, uint64_t num2);
    // TODO(Hakula): Add more operations
};

}  // namespace assets

#endif  // SRC_ASSETS_ALU_H_
