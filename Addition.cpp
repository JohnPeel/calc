#include <cassert>
#include <cmath>
#include <iostream>
#include <iterator>
#include "Addition.h"
#include "Subtraction.h"
#include "Division.h"
#include "Exponentiation.h"
#include "Utility.h"
#include "Variable.h"

double Addition::getValue() {
    return leftSide->getValue() + rightSide->getValue();
}

std::deque<Expression *> Addition::getNumeratorFactors() {
    Expression* leftSide = this->leftSide;
    Expression* rightSide = this->rightSide;

    std::deque<Expression*> ret;
    std::deque<Expression*> leftFactors = leftSide->getFactors();
    std::deque<Expression*> rightFactors = rightSide->getFactors();
    std::deque<Expression*> commonFactors = getCommonFactors(leftFactors, rightFactors);

    Expression* commonFactor = multiplyFactors(commonFactors, true)->simplify();
    if (*commonFactor != *one) {
        ret.push_back(commonFactor);

        leftSide = (new Division(leftSide, commonFactor))->simplify();
        rightSide = (new Division(rightSide, commonFactor))->simplify();
    }

    Integer* leftSideInt = dynamic_cast<Integer*>(leftSide);
    Integer* rightSideInt = dynamic_cast<Integer*>(rightSide);
    rightFactors = rightSide->getFactors();

    Expression* newAdd = new Addition(leftSide, rightSide);

    bool isSub = false;
    for (Expression* factor : rightFactors)
        if (factor->hasValue() && (factor->getValue() == -1))
            isSub = !isSub;
    if (isSub)
        newAdd = new Subtraction(leftSide, (new Multiplication(rightSide, negOne))->simplify());

    if (leftSideInt && rightSideInt)
        newAdd = newAdd->simplify(); // NOTE: This won't create an infinite loop because they are both Int.
    ret.push_back(newAdd);

    return ret;
}

std::deque<Expression*> Addition::getAdditiveTerms() {
    std::deque<Expression*> ret;

    std::deque<Expression*> left = leftSide->getAdditiveTerms();
    std::deque<Expression*> right = rightSide->getAdditiveTerms();

    ret.insert(ret.end(), left.begin(), left.end());
    ret.insert(ret.end(), right.begin(), right.end());

    return ret;
}

Expression* Addition::simplify() {
    Integer* leftInt = dynamic_cast<Integer*>(leftSide->simplify());
    Integer* rightInt = dynamic_cast<Integer*>(rightSide->simplify());

    if (leftInt && rightInt)
        return new Integer((int)getValue());

    std::deque<Expression*> allTerms;
    for (Expression* term : getAdditiveTerms()) {
        term = term->simplify();
        if (*term != *zero)
            allTerms.push_back(term);
    }

    std::deque<Expression*> terms, intTerms;
    std::remove_copy_if(allTerms.begin(), allTerms.end(), std::back_inserter(terms), [](Expression* e){ return (dynamic_cast<Integer*>(e) != NULL); });
    std::remove_copy_if(allTerms.begin(), allTerms.end(), std::back_inserter(intTerms), [](Expression* e){ return (dynamic_cast<Integer*>(e) == NULL); });

    if (intTerms.size() > 1)
        terms.push_back(addTerms(intTerms)->simplify());
    else
        terms = allTerms;

    if (terms.size() > 2)
        if (terms[0]->isNeg())
            for (size_t i = 1; i < terms.size(); i++)
                if (!terms[i]->isNeg())
                    std::swap(terms[0], terms[i]);

    Expression* expr = addTerms(terms);

    std::deque<Expression*> numFactors = expr->getNumeratorFactors();
    std::deque<Expression*> denFactors = expr->getDenominatorFactors();

    if ((denFactors.size() == 0) || ((denFactors.size() == 1) && (*(denFactors.front()) == *one))) {
        if (numFactors.size() > 1)
            expr = multiplyFactors(numFactors)->simplify();
        else
            expr = numFactors.front();
    } else
        expr = (new Division(multiplyFactors(numFactors), multiplyFactors(denFactors)))->simplify();

    Addition* add = dynamic_cast<Addition*>(expr);
    if (add) {
        terms = add->getAdditiveTerms();
        switch (terms.size()) {
            case 2: { // x^3 + a^3, x^3 - a^3, x^2 - a^2
                Expression* left = terms[0];
                Expression* right = terms[1];

                Expression* leftCubeRt = (new NthRoot(three, left))->simplify();
                Expression* rightCubeRt = (new NthRoot(three, right))->simplify();

                bool cubeRt = true;
                for (Expression* term : leftCubeRt->getNumeratorFactors())
                    cubeRt = cubeRt && (dynamic_cast<NthRoot*>(term) == NULL);
                for (Expression* term : rightCubeRt->getNumeratorFactors())
                    cubeRt = cubeRt && (dynamic_cast<NthRoot*>(term) == NULL);

                if (cubeRt) {
                    Expression* leftCRtSq = new Exponentiation(leftCubeRt, two);
                    Expression* rightCRtSq = new Exponentiation(rightCubeRt, two);
                    Expression* leftTerm = new Addition(leftCubeRt, rightCubeRt);

                    std::deque<Expression*> rightTerms;
                    rightTerms.push_back(leftCRtSq);
                    rightTerms.push_back(new Multiplication(negOne, new Multiplication(leftCubeRt, rightCubeRt)));
                    rightTerms.push_back(rightCRtSq);

                    return (new Multiplication(leftTerm, addTerms(rightTerms)))->simplify();
                }

                if (left->isNeg() != right->isNeg()) {
                    if (left->isNeg())
                        left = new Multiplication(negOne, left);
                    if (right->isNeg())
                        right = new Multiplication(negOne, right);

                    Expression* leftSqrRt = (new NthRoot(two, left))->simplify();
                    Expression* rightSqrRt = (new NthRoot(two, right))->simplify();

                    bool sqrRt = true;
                    for (Expression* term : leftSqrRt->getNumeratorFactors())
                        sqrRt = sqrRt && (dynamic_cast<NthRoot*>(term) == NULL);
                    for (Expression* term : rightSqrRt->getNumeratorFactors())
                        sqrRt = sqrRt && (dynamic_cast<NthRoot*>(term) == NULL);

                    if (sqrRt) {
                        Expression* leftTerm = new Addition(leftSqrRt, rightSqrRt);
                        Expression* rightTerm = new Subtraction(leftSqrRt, rightSqrRt);

                        return (new Multiplication(leftTerm, rightTerm))->simplify();
                    }
                }
                break;
            }
            case 3: {  // ax^2 + bx + c
                Expression *a = NULL, *b = NULL, *c = NULL, *var = NULL;
                for (int i = 0; i < 3; i++) {
                    Expression* x = terms[i]->simplify();
                    Integer* xInt = dynamic_cast<Integer*>(x);
                    Variable* xVar = dynamic_cast<Variable*>(x);
                    Multiplication* xMul = dynamic_cast<Multiplication*>(x);
                    Exponentiation* xExp = dynamic_cast<Exponentiation*>(x);

                    if (xExp && xExp->getRightSide()->hasValue() && (xExp->getRightSide()->getValue() == 2)) {
                        if (var && (*var != *(xExp->getLeftSide())))
                            break;
                        var = xExp->getLeftSide();
                        a = one;
                    } else if (xMul) {
                        Expression* left = xMul->getLeftSide();
                        Expression* right = xMul->getRightSide();
                        Multiplication* leftMul = dynamic_cast<Multiplication*>(xMul->getLeftSide());
                        Multiplication* rightMul = dynamic_cast<Multiplication*>(xMul->getRightSide());
                        Exponentiation* leftExp = dynamic_cast<Exponentiation*>(xMul->getLeftSide());
                        Exponentiation* rightExp = dynamic_cast<Exponentiation*>(xMul->getRightSide());

                        if (leftMul && (*(leftMul->getRightSide()) == *(leftMul->getLeftSide()))) {
                            if (var && (*var != *(leftMul->getRightSide())))
                                break;
                            var = leftMul->getRightSide();
                            a = xMul->getRightSide();
                        } else if (rightMul && (*(rightMul->getRightSide()) == *(rightMul->getLeftSide()))) {
                            if (var && (*var != *(rightMul->getRightSide())))
                                break;
                            var = rightMul->getRightSide();
                            a = xMul->getLeftSide();
                        }else if (rightExp && rightExp->getRightSide()->hasValue() && (rightExp->getRightSide()->getValue() == 2)) {
                            if (var && (*var != *(rightExp->getLeftSide())))
                                break;
                            var = rightExp->getLeftSide();
                            a = xMul->getLeftSide();
                        } else if (leftExp && leftExp->getRightSide()->hasValue() && (leftExp->getRightSide()->getValue() == 2)) {
                            if (var && (*var != *(leftExp->getLeftSide())))
                                break;
                            var = leftExp->getLeftSide();
                            a = xMul->getRightSide();
                        } else if ((!leftExp) && (!rightExp)) {
                            if ((var && (*var == *left)) || (right->hasValue() && (!left->hasValue())))
                                b = right;
                            if ((var && (*var == *right)) || (left->hasValue() && (!right->hasValue())))
                                b = left;
                        } else
                            return expr;
                    } else if (xVar) {
                        if (var && (*var != *xVar))
                            break;
                        b = one;
                        var = x;
                    } else if (xInt)
                        c = x;
                    else
                        break;
                }

                if (a && b && c && var) {
                    Expression* negB = (new Multiplication(negOne, b))->simplify();
                    Expression* b2 = (new Exponentiation(b, two))->simplify();
                    Expression* twoA = (new Multiplication(two, a))->simplify();
                    Expression* ac = (new Multiplication(a, c))->simplify();
                    Expression* d = (new Subtraction(b2, new Multiplication(new Integer(4), ac)))->simplify();

                    assert(d->hasValue() && dynamic_cast<Integer*>(d));
                    Expression* sqrtD = (new NthRoot(two, d))->simplify();

                    if ((d->getValue() > 0) && (!dynamic_cast<NthRoot*>(sqrtD))) {
                        Expression* firstTerm = (new Subtraction(var, new Division(new Subtraction(negB, sqrtD), twoA)))->simplify();
                        Expression* secondTerm = (new Subtraction(var, new Division(new Addition(negB, sqrtD), twoA)))->simplify();
                        return new Multiplication(firstTerm, secondTerm);
                    } else if (d->getValue() == 0)
                        return (new Exponentiation(new Subtraction(var, new Division(negB, twoA)), two))->simplify();
                }

                break;
            }
            case 4:
                // TODO: Factor by grouping.
                break;
            default:
                break;
        }
    }

    return expr;
}

std::string Addition::getString() {
    std::string ret;

    std::deque<Expression*> terms = getAdditiveTerms();
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
                } else if (term->isNeg()){
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