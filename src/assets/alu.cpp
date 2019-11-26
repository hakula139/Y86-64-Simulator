#include "alu.h"

#include "register.h"

namespace assets {

int64_t ArithmeticLogicUnit::Add(int64_t num1, int64_t num2) {
    int64_t result = num1 + num2;
    SetConditionCode(!result, result < 0,
                     (num1 > 0 && num2 > 0 && result < 0) ||
                         (num1 < 0 && num2 < 0 && result >= 0));
    return result;
}

int64_t ArithmeticLogicUnit::Sub(int64_t num1, int64_t num2) {
    return Add(num1, -num2);
}

int64_t ArithmeticLogicUnit::And(int64_t num1, int64_t num2) {
    int64_t result = num1 & num2;
    SetConditionCode(!result, result < 0, 0);
    return result;
}

int64_t ArithmeticLogicUnit::Xor(int64_t num1, int64_t num2) {
    int64_t result = num1 ^ num2;
    SetConditionCode(!result, result < 0, 0);
    return result;
}

bool ArithmeticLogicUnit::SetConditionCode(bool zf, bool sf, bool of) {
    ConditionCode::Set(ZF, zf);
    ConditionCode::Set(SF, sf);
    ConditionCode::Set(OF, of);
    return true;
}

}  // namespace assets
