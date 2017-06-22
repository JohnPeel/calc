#ifndef CALC_MULTIPLICATION_H
#define CALC_MULTIPLICATION_H

#include "Expression.h"

class Multiplication : public Expression {
protected:
    Expression* leftSide;
    Expression* rightSide;
public:
    Multiplication(Expression*, Expression*);
    double getValue() override;

    Expression* getLeftSide() override;
    Expression* getRightSide() override;

    std::vector<Expression*> getNumeratorFactors() override;

    Expression* simplify() override;
    std::string getString() override;
};


#endif //CALC_MULTIPLICATION_H
