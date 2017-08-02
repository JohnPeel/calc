#include <algorithm>
#include "Division.h"
#include "Multiplication.h"
#include "Integer.h"

double Division::getValue() {
    return leftSide->getValue() / rightSide->getValue();
}

ExpressionList Division::getNumeratorFactors() {
    return leftSide->getFactors();
}

ExpressionList Division::getDenominatorFactors() {
    return rightSide->getFactors();
}

Expression* Division::simplify() {
    Expression* left = leftSide->simplify();
    Expression* right = rightSide->simplify();

    if (*right == *one)
        return left;
    if (*right == *negOne)
        return new Multiplication(negOne, left);

    ExpressionList allNum, allDen;
    for (Expression* term : left->getNumeratorFactors())
        if (*term != *one)
            allNum.push_back(term);
    for (Expression* term : left->getDenominatorFactors())
        if (*term != *one)
            allDen.push_back(term);
    for (Expression* term : right->getNumeratorFactors())
        if (*term != *one)
            allDen.push_back(term);
    for (Expression* term : right->getDenominatorFactors())
        if (*term != *one)
            allNum.push_back(term);

    for (Expression *item : allDen)
        if (*item == *negOne) {
            allNum.push_back(negOne);
            allDen.erase(std::find(allDen.begin(), allDen.end(), item));
        }

    if ((allDen.size() > 1) || ((allDen.size() == 1) && (*allDen[0] != *one))) {
        ExpressionList commonFactors = getCommonFactors(allNum, allDen);

        if (commonFactors.size() >= 1) {
            for (Expression *commonItem : commonFactors) {
                for (Expression *item : allNum)
                    if ((*item == *commonItem) || (*item == *one)) {
                        allNum.erase(std::find(allNum.begin(), allNum.end(), item));
                        break;
                    }
                for (Expression *item : allDen)
                    if ((*item == *commonItem) || (*item == *one)) {
                        allDen.erase(std::find(allDen.begin(), allDen.end(), item));
                        break;
                    }
            }

            if ((allDen.size() == 0) || ((allDen.size() == 1) && (*allDen[0] == *one)))
                return multiplyFactors(allNum, true);

            if (allDen.size() > 0)
                return new Division(multiplyFactors(allNum)->simplify(), multiplyFactors(allDen)->simplify());
        }
    }

    // TODO: Rationalize denominator!

    return this;
}
