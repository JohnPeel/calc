#include "Exponentiation.h"
#include "Integer.h"
#include "Utility.h"
#include <cmath>

Exponentiation::Exponentiation(Expression* leftSide, Expression* rightSide) {
    this->leftSide = leftSide;
    this->rightSide = rightSide;
}

double Exponentiation::getValue() {
    return std::pow(leftSide->getValue(), rightSide->getValue());
}

Expression* Exponentiation::getLeftSide() {
    return leftSide;
}

Expression* Exponentiation::getRightSide() {
    return rightSide;
}

std::vector<Expression*> Exponentiation::getNumeratorFactors() {
    std::vector<Expression*> ret;

    Integer* rightSideInt = dynamic_cast<Integer*>(rightSide);
    if ((rightSideInt) and (rightSideInt->getValue() > 0)) {
        for (int i = 1; i <= rightSideInt->getValue(); i++)
            ret.push_back(leftSide);
    } else
        ret.push_back(this);

    return ret;
}

std::vector<Expression*> Exponentiation::getDenominatorFactors() {
    std::vector<Expression*> ret;

    Integer* rightSideInt = dynamic_cast<Integer*>(rightSide);
    if ((rightSideInt) and (rightSideInt->getValue() < 0)) {
        for (int i = 1; i <= rightSideInt->getValue(); i++)
            ret.push_back(leftSide);
    } else
        ret.push_back(one);

    return ret;
}

Expression* Exponentiation::simplify() {
    Expression* leftSide = this->leftSide->simplify();
    Expression* rightSide = this->rightSide->simplify();

    Integer* rightSideInt = dynamic_cast<Integer*>(rightSide);
    if (rightSideInt) {
        if (rightSideInt->getValue() == 0)
            return one;
        if (rightSideInt->getValue() == 1)
            return leftSide;
        return multiplyFactors(getNumeratorFactors());
    }

    return new Exponentiation(leftSide, rightSide);
}

std::string Exponentiation::getString() {
    Integer* leftSideInt = dynamic_cast<Integer*>(leftSide);
    Integer* rightSideInt = dynamic_cast<Integer*>(rightSide);
    std::string ret;

    if (leftSideInt)
        ret += leftSide->getString();
    else
        ret += "(" + leftSide->getString() + ")";

    ret += " ^ ";

    if (rightSideInt)
        ret += rightSide->getString();
    else
        ret += "(" + rightSide->getString() + ")";

    return ret;
}
