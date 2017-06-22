#include "Addition.h"
#include "Subtraction.h"
#include "Division.h"
#include "Utility.h"

Addition::Addition(Expression* leftSide, Expression* rightSide) {
    this->leftSide = leftSide;
    this->rightSide = rightSide;
}

double Addition::getValue() {
    return leftSide->getValue() + rightSide->getValue();
}

Expression* Addition::getLeftSide() {
    return leftSide;
}

Expression *Addition::getRightSide() {
    return rightSide;
}

#include <iostream>

std::vector<Expression *> Addition::getNumeratorFactors() {
    Expression* leftSide = this->leftSide->simplify();
    Expression* rightSide = this->rightSide->simplify();

    std::vector<Expression*> ret;
    std::vector<Expression*> leftFactors = leftSide->getNumeratorFactors();
    std::vector<Expression*> rightFactors = rightSide->getNumeratorFactors();
    std::vector<Expression*> commonFactors = getCommonFactors(leftFactors, rightFactors);

    Expression* commonFactor = multiplyFactors(commonFactors)->simplify();
    if (*commonFactor != *one) {
        ret.push_back(commonFactor);

        leftSide = (new Division(leftSide, commonFactor))->simplify();
        rightSide = (new Division(rightSide, commonFactor))->simplify();
    }

    Integer* leftSideInt = dynamic_cast<Integer*>(leftSide);
    Integer* rightSideInt = dynamic_cast<Integer*>(rightSide);
    rightFactors = rightSide->getNumeratorFactors();

    Expression* newAdd = new Addition(leftSide, rightSide);

    bool isSub = false;
    for (Expression* factor : rightFactors)
        if (*factor == *negOne)
            isSub = not isSub;
    if (isSub)
        newAdd = new Subtraction(leftSide, (new Multiplication(rightSide, negOne))->simplify());

    if (leftSideInt and rightSideInt)
        newAdd = newAdd->simplify(); // NOTE: This won't create an infinite loop because they are both Int.
    ret.push_back(newAdd);

    return ret;
}

std::vector<Expression*> Addition::getAdditiveTerms() {
    std::vector<Expression*> vect;

    Expression* leftSide = this->leftSide->simplify();
    Expression* rightSide = this->rightSide->simplify();

    Addition* leftSideAdd = dynamic_cast<Addition*>(leftSide);
    Addition* rightSideAdd = dynamic_cast<Addition*>(rightSide);

    if (leftSideAdd) {
        std::vector<Expression*> _temp = leftSideAdd->getAdditiveTerms();
        vect.insert(vect.end(), _temp.begin(), _temp.end());
    } else
        vect.push_back(leftSide);

    if (rightSideAdd) {
        std::vector<Expression*> _temp = rightSideAdd->getAdditiveTerms();
        vect.insert(vect.end(), _temp.begin(), _temp.end());
    } else
        vect.push_back(rightSide);

    return vect;
}

Expression* Addition::simplify() {
    Expression* expr = commutativeSimplify<Addition>(this, leftSide->simplify(), rightSide->simplify());

    std::vector<Expression*> numFactors = expr->getNumeratorFactors();
    std::vector<Expression*> denFactors = expr->getDenominatorFactors();

    if ((denFactors.size() == 1) and (*(denFactors.front()) == *one)) {
        if (numFactors.size() > 1)
            return multiplyFactors(numFactors)->simplify();
        return numFactors.front();
    } else
        return (new Division(multiplyFactors(numFactors), multiplyFactors(denFactors)))->simplify();
}

std::string Addition::getString() {
    return leftSide->getString() + " + " + rightSide->getString();
}
