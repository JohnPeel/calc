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

ExpressionList Integer::getNumeratorFactors() {
    std::vector<int> factors = getPrimeFactors(value);

    ExpressionMap map;
    for (int factor : factors)
        map[new Integer(factor)]++;

    return factorMapToList(map);
}
