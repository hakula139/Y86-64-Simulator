#include "alu.h"

#include "register.h"

namespace assets {

int64_t ArithmeticLogicUnit::AddSigned(int64_t num1, int64_t num2) {
    int64_t result = num1 + num2;
    // signed overflow
    ConditionCode::Set(ZF, !result);
    ConditionCode::Set(SF, result < 0);
    ConditionCode::Set(OF, (num1 > 0 && num2 > 0 && result < 0) ||
                               (num1 < 0 && num2 < 0 && result >= 0));
    return result;
}

uint64_t ArithmeticLogicUnit::AddUnsigned(uint64_t num1, uint64_t num2) {
    uint64_t result = num1 + num2;
    // unsigned overflow
    ConditionCode::Set(ZF, !result);
    ConditionCode::Set(CF, result < num1 && result < num2);
    return result;
}

int64_t ArithmeticLogicUnit::SubSigned(int64_t num1, int64_t num2) {
    return AddSigned(num1, -num2);
}

uint64_t ArithmeticLogicUnit::SubUnsigned(uint64_t num1, uint64_t num2) {
    return AddUnsigned(num1, -num2);
}

uint64_t ArithmeticLogicUnit::Inc(uint64_t num) { return AddUnsigned(num, 1); }

uint64_t ArithmeticLogicUnit::Dec(uint64_t num) { return SubUnsigned(num, 1); }

int64_t ArithmeticLogicUnit::And(int64_t num1, int64_t num2) {
    int64_t result = num1 & num2;
    ConditionCode::Set(ZF, !result);
    ConditionCode::Set(SF, result < 0);
    ConditionCode::Set(OF, 0);
    return result;
}

int64_t ArithmeticLogicUnit::Or(int64_t num1, int64_t num2) {
    int64_t result = num1 | num2;
    ConditionCode::Set(ZF, !result);
    ConditionCode::Set(SF, result < 0);
    ConditionCode::Set(OF, 0);
    return result;
}

int64_t ArithmeticLogicUnit::Xor(int64_t num1, int64_t num2) {
    int64_t result = num1 ^ num2;
    ConditionCode::Set(ZF, !result);
    ConditionCode::Set(SF, result < 0);
    ConditionCode::Set(OF, 0);
    return result;
}

int64_t ArithmeticLogicUnit::ArithmeticLeftShift(int64_t num, size_t count) {
    int64_t result = num << count;
    ConditionCode::Set(ZF, !result);
    ConditionCode::Set(SF, result < 0);
    ConditionCode::Set(OF, (num < 0 && result >= 0) || (num > 0 && result < 0));
    return result;
}

int64_t ArithmeticLogicUnit::ArithmeticRightShift(int64_t num, size_t count) {
    int64_t result = num >> count;
    ConditionCode::Set(ZF, !result);
    ConditionCode::Set(SF, result < 0);
    ConditionCode::Set(OF, 0);
    return result;
}

uint64_t ArithmeticLogicUnit::LogicalLeftShift(uint64_t num, size_t count) {
    uint64_t result = num << count;
    ConditionCode::Set(ZF, !result);
    ConditionCode::Set(CF, result < num);
    return result;
}

uint64_t ArithmeticLogicUnit::LogicalRightShift(uint64_t num, size_t count) {
    uint64_t result = num >> count;
    ConditionCode::Set(ZF, !result);
    ConditionCode::Set(CF, 0);
    return result;
}

}  // namespace assets
