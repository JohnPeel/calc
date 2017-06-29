#ifndef CALC_DIVISION_H
#define CALC_DIVISION_H

#include "Utility.h"

class Division : public BiOpExpression {
public:
    Division(Expression* left, Expression* right) : BiOpExpression(left, right) {};
    double getValue() override;

    ExpressionList getNumeratorFactors() override;
    ExpressionList getDenominatorFactors() override;
    ExpressionList getFactors() override { return getNumeratorFactors(); };

    bool hasValue() override { return hasExactValue(); };
    bool hasExactValue() override { return getRightSide()->getValue() != 0; };

    Expression* simplify() override;
    std::string getOpString() override { return "/"; };
    std::string getTypeString() override { return "Division"; };
};


#endif //CALC_DIVISION_H
