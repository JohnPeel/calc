#ifndef CALC_SUBTRACTION_H
#define CALC_SUBTRACTION_H

#include "Expression.h"
#include "Addition.h"
#include "Integer.h"
#include "Multiplication.h"

class Subtraction : public Addition {
public:
    Subtraction(Expression* leftSide, Expression* rightSide) : Addition(leftSide, new Multiplication(negOne, rightSide)) { };

    std::string getOpString() override { return "-"; }

    Expression* getRightSide() override {
        Multiplication* right = dynamic_cast<Multiplication*>(rightSide);

        if ((right) && (*(right->getLeftSide()) == *negOne))
            return right->getRightSide();

        return rightSide;
    };
};

#endif //CALC_SUBTRACTION_H
