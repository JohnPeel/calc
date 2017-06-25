#ifndef CALC_DIVISION_H
#define CALC_DIVISION_H

#include "Expression.h"

class Division : public BiOpExpression {
public:
    Division(Expression* left, Expression* right) : BiOpExpression(left, right) {};
    double getValue() override;

    std::deque<Expression*> getNumeratorFactors() override;
    std::deque<Expression*> getDenominatorFactors() override;
    std::deque<Expression*> getFactors() override { return getNumeratorFactors(); };

    bool hasValue() override { return hasExactValue(); };
    bool hasExactValue() override { return getRightSide()->getValue() != 0; };

    Expression* simplify() override;
    std::string getOpString() override { return "/"; };
};


#endif //CALC_DIVISION_H
