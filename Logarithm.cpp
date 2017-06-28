#include "Logarithm.h"
#include "Utility.h"
#include "Exponentiation.h"
#include "Division.h"
#include "Subtraction.h"

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

    std::map<Expression*, int, ExpressionComp> leftFactorMap = dequeToFactorMap(leftSide->getFactors());
    std::map<Expression*, int, ExpressionComp> rightFactorMap = dequeToFactorMap(rightSide->getFactors());

    int count = (int)rightFactorMap.size();
    for (auto& factor : rightFactorMap)
        count = std::min(count, leftFactorMap[factor.first] / rightFactorMap[factor.first]);

    std::deque<Expression*> terms;
    if (count > 0) {
        for (auto& factor : rightFactorMap)
            leftFactorMap[factor.first] -= count * rightFactorMap[factor.first];

        leftSide = multiplyFactors(factorMapToDeque(leftFactorMap))->simplify();
    }

    std::deque<Expression*> addFactors = leftSide->getNumeratorFactors();
    for (Expression* term : addFactors)
        terms.push_back(new Logarithm(term, rightSide));

    std::deque<Expression*> subFactors = leftSide->getDenominatorFactors();
    for (Expression* term : subFactors)
        terms.push_back(new Multiplication(negOne, new Logarithm(term, rightSide)));

    if (count > 0)
        terms.push_back(new Integer(count));

    if (terms.size() > 1)
        return addTerms(terms)->simplify();

    return this;
}
