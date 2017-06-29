#include "Logarithm.h"
#include "Integer.h"
#include "Multiplication.h"
#include <cmath>

double Logarithm::getValue() {
    return std::log(getRightSide()->getValue()) / std::log(getLeftSide()->getValue());
}

Expression *Logarithm::simplify() {
    Expression* leftSide = this->leftSide->simplify();
    Expression* rightSide = this->rightSide->simplify();

    if (*leftSide == *one)
        return zero;

    if (*leftSide == *rightSide)
        return one;

    ExpressionMap leftFactorMap = listToFactorMap(leftSide->getFactors());
    ExpressionMap rightFactorMap = listToFactorMap(rightSide->getFactors());

    int count = (int)rightFactorMap.size();
    for (auto& factor : rightFactorMap)
        count = std::min(count, leftFactorMap[factor.first] / rightFactorMap[factor.first]);

    ExpressionList terms;
    if (count > 0) {
        for (auto& factor : rightFactorMap)
            leftFactorMap[factor.first] -= count * rightFactorMap[factor.first];

        leftSide = multiplyFactors(factorMapToList(leftFactorMap))->simplify();
    }

    ExpressionList addFactors = leftSide->getNumeratorFactors();
    for (Expression* term : addFactors)
        terms.push_back(new Logarithm(term, rightSide));

    ExpressionList subFactors = leftSide->getDenominatorFactors();
    for (Expression* term : subFactors)
        terms.push_back(new Multiplication(negOne, new Logarithm(term, rightSide)));

    if (count > 0)
        terms.push_back(new Integer(count));

    if (terms.size() > 1)
        return addTerms(terms)->simplify();

    return this;
}
