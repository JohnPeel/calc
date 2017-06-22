#ifndef CALC_SUBTRACTION_H
#define CALC_SUBTRACTION_H

#include "Expression.h"
#include "Addition.h"
#include "Integer.h"
#include "Multiplication.h"

class Subtraction : public Addition {
public:
    Subtraction(Expression* leftSide, Expression* rightSide) : Addition(leftSide, new Multiplication(negOne, rightSide)) { };
    std::string getString() override { return leftSide->getString() + " - " + rightSide->getRightSide()->getString(); };
};

#endif //CALC_SUBTRACTION_H
