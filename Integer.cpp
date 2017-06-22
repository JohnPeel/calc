#include "Integer.h"
#include "Prime.h"

double Integer::getValue() {
    return value;
}

Integer::Integer(int value) {
    this->value = value;
}

std::string Integer::getString() {
    return std::to_string(value);
}

std::vector<Expression *> Integer::getNumeratorFactors() {
    std::vector<Expression*> ret;

    if (value == 1) {
        ret.push_back(one);
        return ret;
    }

    if (value < 0)
        ret.push_back(negOne);

    std::vector<int> factors = getPrimeFactors(abs(value));
    for (int factor : factors)
        ret.push_back(new Integer(factor));

    return ret;
}
