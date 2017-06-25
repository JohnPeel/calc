#ifndef CALC_ADDITION_H
#define CALC_ADDITION_H

#include "Expression.h"

class Addition : public BiOpExpression {
public:
    Addition(Expression* left, Expression* right) : BiOpExpression(left, right) {};
    double getValue() override;

    std::deque<Expression*> getNumeratorFactors() override;

    std::deque<Expression*> getAdditiveTerms() override;
    Expression* simplify() override;

    std::string getOpString() override { return "+"; };
    std::string getString() override;
};


#endif //CALC_ADDITION_H
