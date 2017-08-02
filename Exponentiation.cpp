#include "Exponentiation.h"
#include "Integer.h"
#include "Multiplication.h"
#include "Division.h"
#include "Variable.h"
#include <cmath>

double Exponentiation::getValue() {
    return std::pow(leftSide->getValue(), rightSide->getValue());
}

ExpressionList Exponentiation::getNumeratorFactors() {
    ExpressionList ret;

    ExpressionList terms = rightSide->getAdditiveTerms();
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

ExpressionList Exponentiation::getDenominatorFactors() {
    ExpressionList ret;

    ExpressionList terms = rightSide->getAdditiveTerms();
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
    if (leftSideInt && *leftSideInt == *one)
        return one;

    if (rightSideInt) {
        if (*rightSideInt == *zero)
            return one;
        if (*rightSideInt == *one)
            return leftSide;
        if (*rightSideInt == *negOne)
            return new Division(one, leftSide);

        if (leftSideInt)
            return multiplyFactors(getFactors())->simplify();
    }

    Division* rightSideDiv = dynamic_cast<Division*>(rightSide);
    if (rightSideDiv) {
        Expression* newLeftSide = new NthRoot(multiplyFactors(rightSideDiv->getDenominatorFactors()), rightSide);
        Expression* newRightSide = multiplyFactors(rightSideDiv->getNumeratorFactors());
        return (new Exponentiation(newLeftSide, newRightSide))->simplify();
    }

    ExpressionList terms = rightSide->getAdditiveTerms();
    if (terms.size() > 1) {
        for (Expression*& term : terms)
            term = new Exponentiation(leftSide, term);
        return multiplyFactors(terms);
    }

    return new Exponentiation(leftSide, rightSide);
}

double NthRoot::getValue() {
    return std::pow(rightSide->getValue(), 1 / leftSide->getValue());
}

ExpressionList NthRoot::getNumeratorFactors() {
    ExpressionList ret;
    if (leftSide->hasValue()) {
        if (leftSide->getValue() > 0)
            ret.push_back(this);
        else
            ret.push_back(one);
    } else
        ret.push_back(this);
    return ret;
}

ExpressionList NthRoot::getDenominatorFactors() {
    ExpressionList ret;
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

    if ((rightSideInt) && (*rightSideInt == *one))
        return one;

    if (leftSideInt) {
        int multiplicity = abs((int)leftSideInt->getValue());
        ExpressionMap factors = listToFactorMap(rightSide->getFactors());

        ExpressionList terms;
        for (auto x : factors)
            while (factors[x.first] >= multiplicity) {
                terms.push_back(x.first);
                factors[x.first] -= multiplicity;
            }

        if ((factors[negOne] % 2 == 1) && ((multiplicity % 2 == 1) || (multiplicity == 2))) {
            if (multiplicity == 2)
                terms.push_back(i);
            else
                terms.push_back(negOne);

            factors.erase(negOne);
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

            if (leftSideInt->getValue() < 0)
                outsideExp = new Division(one, outsideExp);

            if (*insideExp == *one)
                return outsideExp->simplify();

            return (new Multiplication(outsideExp, new NthRoot(leftSide, insideExp)))->simplify();
        }
    }

    return new NthRoot(leftSide, rightSide);
}
