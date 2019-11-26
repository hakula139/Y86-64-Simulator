#ifndef SRC_ASSETS_ALU_H_
#define SRC_ASSETS_ALU_H_

#include <cstdint>

namespace assets {

enum AluFunctionMap : int { ADDQ = 0x0, SUBQ = 0x1, ANDQ = 0x2, XORQ = 0x3 };

enum ConditionMap : int {
    C_AW = 0x0,
    C_LE = 0x1,
    C_L  = 0x2,
    C_E  = 0x3,
    C_NE = 0x4,
    C_GE = 0x5,
    C_G  = 0x6
};

// Provides basic operations for integers and sets condition codes respectively
class ArithmeticLogicUnit {
public:
    // Adds 'num2' to 'num1'
    static int64_t Add(int64_t num1, int64_t num2);
    // Subtracts 'num2' from 'num1'
    static int64_t Sub(int64_t num1, int64_t num2);
    // Does the bitwise AND on 'num1' and 'num2'
    static int64_t And(int64_t num1, int64_t num2);
    // Does the bitwise XOR on 'num1' and 'num2'
    static int64_t Xor(int64_t num1, int64_t num2);

protected:
    static bool SetConditionCode(bool zf, bool sf, bool of);
};

}  // namespace assets

#endif  // SRC_ASSETS_ALU_H_
