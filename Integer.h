#ifndef CALC_INTEGER_H
#define CALC_INTEGER_H

#include "Utility.h"

class Integer : public Expression {
protected:
    int value;
public:
    Integer(int);
    double getValue() override;
    bool hasValue() override { return true; };
    bool hasExactValue() override { return true; };

    ExpressionList getNumeratorFactors() override;

    Expression* simplify() override { return this; };
    bool needParenthesis() override { return value < 0; };
    std::string getString() override;
    std::string getTypeString() override { return "Integer"; };
};

// Some Integers used a lot in places.
extern Expression* negOne;
extern Expression* zero;
extern Expression* one;
extern Expression* two;
extern Expression* three;

#endif //CALC_INTEGER_H
