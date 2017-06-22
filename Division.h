#ifndef CALC_DIVISION_H
#define CALC_DIVISION_H

#include "Expression.h"

class Division : public Expression {
protected:
    Expression* leftSide;
    Expression* rightSide;
public:
    Division(Expression*, Expression*);
    double getValue() override;

    Expression* getLeftSide() override;
    Expression* getRightSide() override;

    std::vector<Expression*> getNumeratorFactors() override;
    std::vector<Expression*> getDenominatorFactors() override;

    Expression* simplify() override;
    std::string getString() override;
};


#endif //CALC_DIVISION_H
