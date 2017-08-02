#include <cassert>
#include <cmath>
#include "Addition.h"
#include "Subtraction.h"
#include "Division.h"
#include "Exponentiation.h"
#include "Variable.h"
#include "Prime.h"

double Addition::getValue() {
    return leftSide->getValue() + rightSide->getValue();
}

ExpressionList Addition::getDenominatorFactors() {
    ExpressionList terms = getAdditiveTerms();
    ExpressionList prevTerm = terms.front()->getDenominatorFactors();
    ExpressionList newTerm = prevTerm;
    ExpressionList common;
    terms.pop_front();

    for (Expression* term : terms) {
        ExpressionList nextTerm = term->getDenominatorFactors();
        ExpressionList commonTerms = getCommonFactors(prevTerm, nextTerm);

        prevTerm = nextTerm;
        newTerm.insert(newTerm.end(), nextTerm.begin(), nextTerm.end());
        common.insert(common.end(), commonTerms.begin(), commonTerms.end());
    }

    Expression* term = (new Division(multiplyFactors(newTerm), multiplyFactors(common)))->simplify();
    ExpressionList ret;
    if (*term != *one)
        ret.push_front(term);
    return ret;
}

ExpressionList Addition::getNumeratorFactors() {
    ExpressionList terms = getAdditiveTerms();
    ExpressionList commonNum = terms.front()->getNumeratorFactors();
    ExpressionList commonDen = terms.front()->getDenominatorFactors();
    Expression* outsideDen = multiplyFactors(commonDen);
    terms.pop_front();

    for (Expression* term : terms) {
        commonNum = getCommonFactors(commonNum, term->getNumeratorFactors());
        ExpressionList termDen = term->getDenominatorFactors();
        commonDen = getCommonFactors(commonDen, termDen);
        termDen.push_front(outsideDen);
        outsideDen = new Division(multiplyFactors(termDen), multiplyFactors(commonDen));
    }

    ExpressionMap ret;
    Expression* commonFactor = (new Division(multiplyFactors(commonNum), outsideDen))->simplify();
    ret[commonFactor] = 1;

    Expression* newAdd;
    if (*commonFactor != *one) {
        leftSide = (new Division(leftSide, commonFactor))->simplify();
        rightSide = (new Division(rightSide, commonFactor))->simplify();
        newAdd = (new Addition(leftSide, rightSide))->simplify();
    } else
        newAdd = new Addition(leftSide, rightSide);

    ExpressionList rightFactors = rightSide->getFactors();
    bool isSub = false;
    for (Expression* factor : rightFactors)
        if (*factor == *negOne)
            isSub = !isSub;

    if (isSub)
        newAdd = new Subtraction(leftSide, (new Multiplication(negOne, rightSide))->simplify());

    if ((dynamic_cast<Integer*>(leftSide) != NULL) && (dynamic_cast<Integer*>(rightSide) != NULL))
        newAdd = newAdd->simplify(); // NOTE: This won't create an infinite loop because they are both Int.

    terms = newAdd->getAdditiveTerms();
    if (terms.size() > 1) {
        bool nope = false;
        std::map<Expression*, std::map<int, double>, ExpressionComp> varMap;
        int constant = 0;

        for (Expression* term : terms) {
            term = term->simplify();

            Integer* termInt = NULL;
            if (exprToTypedExpression(term, termInt)) {
                constant += termInt->getValue();
                continue;
            }

            int currentDegree = 0;
            double currentCoefficient = 1;
            Expression* var = NULL;
            for (auto factor : listToFactorMap(term->getFactors())) {
                Variable* factorVar = NULL;
                Integer* factorInt = NULL;

                if (exprToTypedExpression(factor.first, factorVar)) {
                    if (var == NULL)
                        var = factorVar;

                    if (*var != *factorVar)
                        var = new Multiplication(var, factorVar);

                    currentDegree = factor.second;
                } else if (exprToTypedExpression(factor.first, factorInt))
                    currentCoefficient *= pow(factorInt->getValue(), factor.second);
                else {
                    nope = true;
                    break;
                }
            }

            if (nope || var == NULL)
                break;

            if (varMap[var].find(currentDegree) != varMap[var].end())
                nope = true;

            varMap[var][currentDegree] = currentCoefficient;
        }

        if (varMap.size() == 1)
            varMap[varMap.begin()->first][0] = constant;
        // FIXME: How to distribute constant if multiVar?

        if (!nope) {
            for (auto var : varMap) {
                std::map<int, double>& map = varMap[var.first];
                int degreeCount = 0;
                for (auto term : map)
                    degreeCount = std::max(degreeCount, term.first);

                for (int i = 0; i <= degreeCount; i++)
                    if (map.find(i) == map.end())
                        map[i] = 0;

                auto syntheticDivision = [&map](double x, std::map<int, double> &newMap) -> double {
                    int highestDegree = (int)map.size() - 1;
                    newMap[highestDegree - 1] = map[highestDegree];
                    for (int i = highestDegree - 1; i > 0; i--)
                        newMap[i - 1] = map[i] + (newMap[i] * x);
                    return map[0] + (newMap[0] * x);
                };

                if (degreeCount > 1) {
                    for (;;) {
                        int oldDegree = degreeCount;
                        int p = (int)map[0];
                        int q = (int)map[degreeCount];

                        for (int pfactor : getPrimeFactors(p))
                            for (int qfactor : getPrimeFactors(q)) {
                                std::map<int, double> newMap;
                                if (syntheticDivision((1.0 * pfactor) / qfactor, newMap) == 0) {
                                    ret[(new Subtraction(var.first, new Division(new Integer(pfactor),
                                                                                 new Integer(qfactor))))->simplify()]++;
                                    map = newMap;
                                    degreeCount--;
                                }

                                newMap.clear();
                                if (syntheticDivision((-1.0 * pfactor) / qfactor, newMap) == 0) {
                                    ret[(new Subtraction(var.first, new Division(new Integer(-pfactor),
                                                                                 new Integer(qfactor))))->simplify()]++;
                                    map = newMap;
                                    degreeCount--;
                                }
                            }

                        if (degreeCount == oldDegree)
                            break; // No factors found.
                    }

                    ExpressionList newTerms;
                    for (int i = map.end()->first; i >= 0; i--)
                        if (map[i] != 0)
                            newTerms.push_back((new Multiplication(new Integer((int)map[i]),
                                                                   new Exponentiation(var.first,
                                                                                      new Integer(i))))->simplify());

                    ret[addTerms(newTerms)]++;
                }
            }
        }
    }

    if (ret.size() == 1)
        ret[newAdd] = 1;

    normalizeFactorMap(ret);
    return factorMapToList(ret);
}

ExpressionList Addition::getAdditiveTerms() {
    ExpressionList ret = leftSide->getAdditiveTerms();
    ExpressionList right = rightSide->getAdditiveTerms();
    ret.insert(ret.end(), right.begin(), right.end());
    return ret;
}

Expression* Addition::simplify() {
    // Combine ints
    ExpressionList terms;
    int intTerm = 0;
    for (Expression*& term : getAdditiveTerms()) {
        term = term->simplify();

        Integer* termInt = NULL;
        if (exprToTypedExpression(term, termInt)) {
            intTerm += (int)termInt->getValue();
            continue;
        }

        bool combined = false;
        ExpressionList termList = term->getFactors();
        for (Expression*& innerTerm : terms) {
            ExpressionList innerTermList = innerTerm->getFactors();
            Expression* common = multiplyFactors(getCommonFactors(termList, innerTermList))->simplify();
            // NOTE: Maybe only combine if common isn't an int?
            if (*common != *one) {
                Expression* newAdd = (new Addition(new Division(innerTerm, common), new Division(term, common)))->simplify();
                innerTerm = (new Multiplication(common, newAdd))->simplify();
                combined = true;
                break;
            }
        }

        if (!combined)
            terms.push_back(term);
    }

    // No need to push intTerm if it's 0 (e.g. x + 0 = x)
    if (intTerm != 0)
        terms.push_back(new Integer(intTerm));

    // Swap negative first term with a positive term
    if (terms.size() > 2)
        if (terms[0]->isNeg())
            for (size_t i = 1; i < terms.size(); i++)
                if (!terms[i]->isNeg())
                    std::swap(terms[0], terms[i]);

    // Factor!
    ExpressionList factors = addTerms(terms)->getFactors();
    if (factors.size() > 1)
        return multiplyFactors(factors)->simplify();
    else
        return multiplyFactors(factors);
}

std::string Addition::getString() {
    std::string ret;

    ExpressionList terms = getAdditiveTerms();
    Expression* firstTerm = terms[0];

    for (Expression* term : terms) {
        std::string term_str = "";
        bool needPar = term->needParenthesis();
        Integer* termInt = dynamic_cast<Integer*>(term);
        Multiplication* termMul = dynamic_cast<Multiplication*>(term);

        if ((termInt != NULL) || (termMul != NULL)) {
            if (term->isNeg() && termInt) {
                Expression* newTerm = (new Multiplication(negOne, term))->simplify();
                term_str = newTerm->getString();
                needPar = newTerm->needParenthesis();
            }

            if (termMul) {
                Expression* leftTerm = termMul->getLeftSide();
                Expression* rightTerm = termMul->getRightSide();

                if (*leftTerm == *negOne) {
                    term_str = rightTerm->getString();
                    needPar = rightTerm->needParenthesis();
                } else if (*rightTerm == *negOne) {
                    term_str = leftTerm->getString();
                    needPar = leftTerm->needParenthesis();
                } else if (term->isNeg()) {
                    if (leftTerm->isNeg() && dynamic_cast<Integer*>(leftTerm))
                        leftTerm = dynamic_cast<Integer*>((new Multiplication(negOne, leftTerm))->simplify());
                    if (rightTerm->isNeg() && dynamic_cast<Integer*>(rightTerm))
                        rightTerm = dynamic_cast<Integer*>((new Multiplication(negOne, rightTerm))->simplify());

                    Expression* newTerm = new Multiplication(leftTerm, rightTerm);
                    term_str = newTerm->getString();
                    needPar = newTerm->needParenthesis();
                }
            }

            if (term_str != "")
                if (term == firstTerm)
                    ret += "   -";
                else
                    ret += " - ";
            else
                ret += " + ";
        } else
            ret += " + ";

        if (term_str == "")
            term_str = term->getString();

        if (needPar)
            ret += "(" + term_str + ")";
        else
            ret += term_str;
    }

    return ret.substr(3, ret.length() - 3);
}