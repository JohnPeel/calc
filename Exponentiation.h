#ifndef CALC_EXPONENTIATION_H
#define CALC_EXPONENTIATION_H

#include "Utility.h"

class Exponentiation : public BiOpExpression {
public:
    Exponentiation(Expression* left, Expression* right) : BiOpExpression(left, right) {};
    double getValue() override;

    ExpressionList getNumeratorFactors() override;
    ExpressionList getDenominatorFactors() override;

    bool needParenthesis() override { return true; };

    Expression* simplify() override;
    std::string getOpString() override { return "^"; };
    std::string getTypeString() override { return "Exponentiation"; };
};

class NthRoot : public BiOpExpression {
public:
    NthRoot(Expression* left, Expression* right) : BiOpExpression(left, right) {};
    double getValue() override;

    ExpressionList getNumeratorFactors() override;
    ExpressionList getDenominatorFactors() override;

    bool hasValue() override { return getRightSide()->getValue() >= 0; };

    bool needParenthesis() override { return true; };

    Expression* simplify() override;
    std::string getOpString() override { return "rt"; };
    std::string getTypeString() override { return "NthRoot"; };
};

#endif //CALC_EXPONENTIATION_H
