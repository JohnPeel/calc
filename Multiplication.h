#ifndef CALC_MULTIPLICATION_H
#define CALC_MULTIPLICATION_H

#include "Utility.h"

class Multiplication : public BiOpExpression {
public:
    Multiplication(Expression* left, Expression* right) : BiOpExpression(left, right) {};
    double getValue() override;

    ExpressionList getNumeratorFactors() override;
    ExpressionList getDenominatorFactors() override;

    Expression* simplify() override;
    bool needParenthesis() override;

    std::string getOpString() override { return "*"; };
    std::string getString() override;
    std::string getTypeString() override { return "Multiplication"; };
};


#endif //CALC_MULTIPLICATION_H
