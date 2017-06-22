#ifndef CALC_INTEGER_H
#define CALC_INTEGER_H

#include "Expression.h"

class Integer : public Expression {
protected:
    int value;
public:
    Integer(int);
    double getValue() override;

    std::vector<Expression *> getNumeratorFactors() override;

    std::string getString() override;
};

static Expression* one = new Integer(1);
static Expression* negOne = new Integer(-1);

#endif //CALC_INTEGER_H
