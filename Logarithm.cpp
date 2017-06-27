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

    if (*rightSide == *one)
        return zero;

    if (*leftSide == *rightSide)
        return one;

    std::map<Expression*, int, ExpressionComp> factorMap = dequeToFactorMap(rightSide->getFactors());
    std::deque<Expression*> factors = leftSide->getFactors();

    int count = factorMap[factors[0]];
    for (Expression* factor : factors)
        count = std::min(count, factorMap[factor]);

    std::deque<Expression*> terms;

    if (count > 0) {
        for (Expression *factor : factors)
            factorMap[factor] -= count;

        rightSide = multiplyFactors(factorMapToDeque(factorMap))->simplify();
    }

    std::deque<Expression*> addFactors = rightSide->getNumeratorFactors();
    for (Expression* term : addFactors)
        terms.push_back(new Logarithm(leftSide, term));
    std::deque<Expression*> subFactors = rightSide->getDenominatorFactors();
    for (Expression* term : subFactors)
        terms.push_back(new Multiplication(negOne, new Logarithm(leftSide, term)));

    if (count > 0)
        terms.push_back(new Integer(count));

    if (terms.size() > 1)
        return addTerms(terms)->simplify();

    return this;
}
