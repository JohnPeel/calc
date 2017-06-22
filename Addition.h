#ifndef CALC_ADDITION_H
#define CALC_ADDITION_H

#include "Expression.h"

class Addition : public Expression {
protected:
    Expression* leftSide;
    Expression* rightSide;
public:
    Addition(Expression*, Expression*);
    double getValue() override;

    Expression* getLeftSide() override;
    Expression* getRightSide() override;

    std::vector<Expression*> getNumeratorFactors() override;

    std::vector<Expression*> getAdditiveTerms() override;
    Expression* simplify() override;

    std::string getString() override;
};


#endif //CALC_ADDITION_H
