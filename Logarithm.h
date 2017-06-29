#ifndef CALC_LOGARITHM_H
#define CALC_LOGARITHM_H

#include "Variable.h"

class Logarithm : public BiOpExpression {
public:
    Logarithm(Expression* left, Expression* right) : BiOpExpression(left, right) {};

    double getValue() override;
    bool hasValue() override { return getRightSide()->getValue() > 0; };
    Expression* simplify() override;

    //bool needParenthesis() override { return (*leftSide != *e); };
    std::string getOpString() override { return "log"; };
    std::string getString() override {
        std::string right = getRightSide()->getString();
        if (rightSide->needParenthesis())
            right = "(" + right + ")";

        if (*leftSide == *e)
            return "ln " + right;
        else
            return OpExpression::getString() + " " + right;
    }
    std::string getTypeString() override { return "Logarithm"; };
};

class LogNatural : public Logarithm {
public:
    LogNatural(Expression* right) : Logarithm(e, right) {};
    std::string getOpString() override { return "ln"; }
    std::string getTypeString() override { return "Natural Log"; };
};


#endif //CALC_LOGARITHM_H
