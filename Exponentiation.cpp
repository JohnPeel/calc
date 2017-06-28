#include "Exponentiation.h"
#include "Integer.h"
#include "Utility.h"
#include "Division.h"
#include "Variable.h"
#include <cmath>
#include <map>
#include <iostream>

double Exponentiation::getValue() {
    return std::pow(leftSide->getValue(), rightSide->getValue());
}

std::deque<Expression*> Exponentiation::getNumeratorFactors() {
    std::deque<Expression*> ret;

    std::deque<Expression*> terms = rightSide->getAdditiveTerms();
    for (Expression* term : terms) {
        Integer* termInt = dynamic_cast<Integer*>(term);

        if (termInt) {
            if (termInt->getValue() > 0)
                for (int i = 1; i <= termInt->getValue(); i++)
                    ret.push_back(leftSide);
        } else
            ret.push_back(new Exponentiation(leftSide, term));
    }

    return ret;
}

std::deque<Expression*> Exponentiation::getDenominatorFactors() {
    std::deque<Expression*> ret;

    std::deque<Expression*> terms = rightSide->getAdditiveTerms();
    for (Expression* term : terms) {
        Integer* termInt = dynamic_cast<Integer*>(term);

        if (termInt) {
            if (termInt->getValue() < 0)
                for (int i = 1; i <= abs((int)termInt->getValue()); i++)
                    ret.push_back(leftSide);
        }
    }

    return ret;
}

Expression* Exponentiation::simplify() {
    Expression* leftSide = this->leftSide->simplify();
    Expression* rightSide = this->rightSide->simplify();

    Integer* leftSideInt = dynamic_cast<Integer*>(leftSide);
    Integer* rightSideInt = dynamic_cast<Integer*>(rightSide);
    if (rightSideInt) {
        if (rightSideInt->getValue() == 0)
            return one;
        if (rightSideInt->getValue() == 1)
            return leftSide;

        if (leftSideInt)
            return multiplyFactors(getNumeratorFactors())->simplify();
    }

    return new Exponentiation(leftSide, rightSide);
}

double NthRoot::getValue() {
    return std::pow(rightSide->getValue(), 1 / leftSide->getValue());
}

std::deque<Expression *> NthRoot::getNumeratorFactors() {
    std::deque<Expression*> ret;
    if (leftSide->hasValue()) {
        if (leftSide->getValue() > 0)
            ret.push_back(this);
        else
            ret.push_back(one);
    } else
        ret.push_back(this);
    return ret;
}

std::deque<Expression*> NthRoot::getDenominatorFactors() {
    std::deque<Expression*> ret;
    if (leftSide->hasValue())
        if (leftSide->getValue() < 0)
            ret.push_back(this);
    return ret;
}

Expression* NthRoot::simplify() {
    Expression* leftSide = this->leftSide->simplify();
    Expression* rightSide = this->rightSide->simplify();

    Division* leftSideDiv = dynamic_cast<Division*>(leftSide);
    if (leftSideDiv) {
        Expression* newLeftSide = multiplyFactors(leftSideDiv->getNumeratorFactors());
        Expression* newRightSide = new Exponentiation(rightSide, multiplyFactors(leftSideDiv->getDenominatorFactors()));
        return (new NthRoot(newLeftSide, newRightSide))->simplify();
    }

    Integer* leftSideInt = dynamic_cast<Integer*>(leftSide);
    Integer* rightSideInt = dynamic_cast<Integer*>(rightSide);

    if (rightSideInt) {
        if (*rightSideInt == *one)
            return one;

        if (*rightSideInt == *negOne)
            if (leftSideInt)
                return (Expression *)(((int)leftSideInt->getValue() % 2 == 0) ? i : negOne);
    }

    if (leftSideInt) {
        int multiplicity = (int)leftSideInt->getValue();
        std::map<Expression*, int, ExpressionComp> factors = dequeToFactorMap(rightSide->getFactors());

        std::deque<Expression*> terms;
        for (auto x : factors)
            if (*x.first != *negOne)
                while (factors[x.first] >= multiplicity) {
                    terms.push_back(x.first);
                    factors[x.first] -= multiplicity;
                }
            else if ((multiplicity % 2 == 0) && (x.second % 4 != 0)) {
                switch (x.second % 4) {
                    case 1:
                        terms.push_back(i);
                        break;
                    case 2:
                        terms.push_back(negOne);
                        break;
                    case 3:
                        terms.push_back(negOne);
                        terms.push_back(i);
                        break;
                }
                factors[x.first] = 0;
            }

        Expression* outsideExp = multiplyFactors(terms)->simplify();
        if (*outsideExp != *one) {
            terms.clear();

            for (auto x : factors)
                while (factors[x.first] > 0) {
                    terms.push_back(x.first);
                    factors[x.first]--;
                }

            Expression* insideExp = multiplyFactors(terms)->simplify();

            if (*insideExp == *one)
                return outsideExp->simplify();
            if (*insideExp == *negOne) {
                if (multiplicity % 2 == 0)
                    return (new Multiplication(outsideExp, i))->simplify();
                else
                    return (new Multiplication(negOne, outsideExp))->simplify();
            }

            return (new Multiplication(outsideExp, new NthRoot(leftSide, insideExp)))->simplify();
        }
    }

    return new NthRoot(leftSide, rightSide);
}
