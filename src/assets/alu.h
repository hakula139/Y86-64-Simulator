#ifndef SRC_ASSETS_ALU_H_
#define SRC_ASSETS_ALU_H_

#include <cstdint>

namespace assets {

// Provides basic operations for integers and sets condition codes respectively
class ArithmeticLogicUnit {
public:
    // Adds 'num2' to 'num1' (both are signed)
    static int64_t AddSigned(int64_t num1, int64_t num2);
    // Adds 'num2' to 'num1' (both are unsigned)
    static uint64_t AddUnsigned(uint64_t num1, uint64_t num2);
    // Subtracts 'num2' from 'num1' (both are signed)
    static int64_t SubSigned(int64_t num1, int64_t num2);
    // Subtracts 'num2' from 'num1' (both are unsigned)
    static uint64_t SubUnsigned(uint64_t num1, uint64_t num2);
    // Increases 'num' by 1 (unsigned)
    static uint64_t Inc(uint64_t num);
    // Decreases 'num' by 1 (unsigned)
    static uint64_t Dec(uint64_t num);
    // Does the bitwise AND on 'num1' and 'num2' (signed)
    static int64_t And(int64_t num1, int64_t num2);
    // Does the bitwise OR on 'num1' and 'num2' (signed)
    static int64_t Or(int64_t num1, int64_t num2);
    // Does the bitwise XOR on 'num1' and 'num2' (signed)
    static int64_t Xor(int64_t num1, int64_t num2);
    // Left-shifts 'num' by 'count' bits (signed)
    static int64_t ArithmeticLeftShift(int64_t num, size_t count);
    // Right-shifts 'num' by 'count' bits (signed)
    static int64_t ArithmeticRightShift(int64_t num, size_t count);
    // Left-shifts 'num' by 'count' bits (unsigned)
    static uint64_t LogicalLeftShift(uint64_t num, size_t count);
    // Right-shifts 'num' by 'count' bits (unsigned)
    static uint64_t LogicalRightShift(uint64_t num, size_t count);
};

}  // namespace assets

#endif  // SRC_ASSETS_ALU_H_
