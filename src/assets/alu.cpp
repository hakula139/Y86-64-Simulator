#include "alu.h"

#include "register.h"

namespace assets {

int64_t ArithmeticLogicUnit::AddSigned(int64_t num1, int64_t num2) {
    int64_t result = num1 + num2;
    ConditionCode::Set(OF, (num1 > 0 && num2 > 0 && result < 0) ||
                               (num1 < 0 && num2 < 0 && result >= 0));
    ConditionCode::Set(SF, result < 0);
    ConditionCode::Set(ZF, !result);
    return result;
}

uint64_t ArithmeticLogicUnit::AddUnsigned(uint64_t num1, uint64_t num2) {
    uint64_t result = num1 + num2;
    ConditionCode::Set(CF, result < num1 && result < num2);
    ConditionCode::Set(ZF, !result);
    return result;
}

}  // namespace assets
