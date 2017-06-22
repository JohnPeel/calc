#ifndef CALC_EXPONENTIATION_H
#define CALC_EXPONENTIATION_H

#include "Expression.h"

class Exponentiation : public Expression {
protected:
    Expression* leftSide;
    Expression* rightSide;
public:
    Exponentiation(Expression*, Expression*);
    double getValue() override;

    Expression *getLeftSide() override;
    Expression *getRightSide() override;

    std::vector<Expression*> getNumeratorFactors() override;
    std::vector<Expression*> getDenominatorFactors() override;

    Expression* simplify() override;
    std::string getString() override;

};

class NthRoot : public Expression {

};

#endif //CALC_EXPONENTIATION_H
