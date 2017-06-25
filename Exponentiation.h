#ifndef CALC_EXPONENTIATION_H
#define CALC_EXPONENTIATION_H

#include "Expression.h"

class Exponentiation : public BiOpExpression {
public:
    Exponentiation(Expression* left, Expression* right) : BiOpExpression(left, right) {};
    double getValue() override;

    std::deque<Expression*> getNumeratorFactors() override;
    std::deque<Expression*> getDenominatorFactors() override;

    bool needParenthesis() override { return true; };

    Expression* simplify() override;
    std::string getOpString() override { return "^"; };

};

class NthRoot : public BiOpExpression {
public:
    NthRoot(Expression* left, Expression* right) : BiOpExpression(left, right) {};
    double getValue() override;

    std::deque<Expression*> getNumeratorFactors() override;
    std::deque<Expression*> getDenominatorFactors() override;

    bool hasValue() override { return getRightSide()->getValue() >= 0; };

    bool needParenthesis() override { return true; };

    Expression* simplify() override;
    std::string getOpString() override { return "rt"; };
};

#endif //CALC_EXPONENTIATION_H
