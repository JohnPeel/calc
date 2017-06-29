#ifndef CALC_ADDITION_H
#define CALC_ADDITION_H

#include "Utility.h"

class Addition : public BiOpExpression {
public:
    Addition(Expression* left, Expression* right) : BiOpExpression(left, right) {};
    double getValue() override;

    ExpressionList getNumeratorFactors() override;

    ExpressionList getAdditiveTerms() override;
    Expression* simplify() override;

    std::string getOpString() override { return "+"; };
    std::string getString() override;
    std::string getTypeString() override { return "Addition"; };
};


#endif //CALC_ADDITION_H
