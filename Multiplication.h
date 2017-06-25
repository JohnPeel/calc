#ifndef CALC_MULTIPLICATION_H
#define CALC_MULTIPLICATION_H

#include "Expression.h"

class Multiplication : public BiOpExpression {
public:
    Multiplication(Expression* left, Expression* right) : BiOpExpression(left, right) {};
    double getValue() override;

    std::deque<Expression*> getNumeratorFactors() override;

    Expression* simplify() override;
    bool needParenthesis() override;
    std::string getString() override;
    std::string getOpString() override { return "*"; };
};


#endif //CALC_MULTIPLICATION_H
