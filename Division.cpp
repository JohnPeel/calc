#include "Division.h"
#include "Utility.h"

Division::Division(Expression* leftSide, Expression* rightSide) {
    this->leftSide = leftSide;
    this->rightSide = rightSide;
}

double Division::getValue() {
    return leftSide->getValue() / rightSide->getValue();
}

Expression *Division::getLeftSide() {
    return leftSide;
}

Expression *Division::getRightSide() {
    return rightSide;
}

std::vector<Expression*> Division::getNumeratorFactors() {
    std::vector<Expression*> left, right;
    left = leftSide->getNumeratorFactors();
    right = rightSide->getDenominatorFactors();
    left.insert(left.end(), right.begin(), right.end());
    return left;
}

std::vector<Expression*> Division::getDenominatorFactors() {
    std::vector<Expression*> left, right;
    left = leftSide->getDenominatorFactors();
    right = rightSide->getNumeratorFactors();
    left.insert(left.begin(), right.begin(), right.end());
    return left;
}

Expression *Division::simplify() {
    std::vector<Expression*> num = getNumeratorFactors();
    std::vector<Expression*> den = getDenominatorFactors();
    std::vector<Expression*> commonFactors = getCommonFactors(num, den);

    for (Expression* commonItem : commonFactors) {
        for (Expression* item : num)
            if (item->getValue() == commonItem->getValue()) {
                num.erase(std::find(num.begin(), num.end(), item));
                break;
            }
        for (Expression* item : den)
            if (item->getValue() == commonItem->getValue()) {
                den.erase(std::find(den.begin(), den.end(), item));
                break;
            }
    }

    if (den.size() == 0)
        return multiplyFactors(num)->simplify();

    if ((den.size() == 1) and (*(den.front()) == *negOne)) {
        num.push_back((Expression *&&) den.front());
        return multiplyFactors(num)->simplify();
    }

    // TODO: Rationalize denominator!

    return new Division(multiplyFactors(num)->simplify(), multiplyFactors(den)->simplify());
}

std::string Division::getString() {
    Integer* leftSideInt = dynamic_cast<Integer*>(leftSide);
    Integer* rightSideInt = dynamic_cast<Integer*>(rightSide);
    std::string ret;

    if (leftSideInt)
        ret += leftSide->getString();
    else
        ret += "(" + leftSide->getString() + ")";

    ret += " / ";

    if (rightSideInt)
        ret += rightSide->getString();
    else
        ret += "(" + rightSide->getString() + ")";

    return ret;
}
