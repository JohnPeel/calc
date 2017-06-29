#ifndef CALC_SUBTRACTION_H
#define CALC_SUBTRACTION_H

#include "Integer.h"
#include "Addition.h"
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
    }

    std::string getTypeString() override { return "Subtraction"; };
};

#endif //CALC_SUBTRACTION_H
