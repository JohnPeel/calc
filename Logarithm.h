#ifndef CALC_LOGARITHM_H
#define CALC_LOGARITHM_H

#include "Expression.h"
#include "Variable.h"

class Logarithm : public BiOpExpression {
public:
    Logarithm(Expression* left, Expression* right) : BiOpExpression(left, right) {};

    double getValue() override;
    bool hasValue() override { return getRightSide()->getValue() > 0; };
    Expression* simplify() override;

    //bool needParenthesis() override { return (*leftSide != *e); };
    std::string getOpString() override { return "log"; };
    std::string getString() {
        std::string right = getRightSide()->getString();
        if (rightSide->needParenthesis())
            right = "(" + right + ")";

        if (*leftSide == *e)
            return "ln " + right;
        else
            return OpExpression::getString() + " " + right;
    }
};

class LogNatural : public Logarithm {
public:
    LogNatural(Expression* right) : Logarithm(e, right) {};
    std::string getOpString() override { return "ln"; }
};


#endif //CALC_LOGARITHM_H
