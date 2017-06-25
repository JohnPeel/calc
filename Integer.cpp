#include "Integer.h"
#include "Prime.h"

Expression* negOne = new Integer(-1);
Expression* zero = new Integer(0);
Expression* one = new Integer(1);
Expression *two = new Integer(2);
Expression *three = new Integer(3);

double Integer::getValue() {
    return value;
}

Integer::Integer(int value) {
    this->value = value;
}

std::string Integer::getString() {
    return std::to_string(value);
}

std::deque<Expression *> Integer::getNumeratorFactors() {
    std::deque<Expression*> ret;

    if (value == 0) {
        ret.push_back(zero);
        return ret;
    }

    if (value == 1) {
        ret.push_back(one);
        return ret;
    }

    std::vector<int> factors = getPrimeFactors(value);
    for (int factor : factors)
        ret.push_back(new Integer(factor));

    return ret;
}
