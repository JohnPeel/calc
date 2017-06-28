#include "Expression.h"
#include "Integer.h"
#include "Utility.h"
#include "Division.h"

double Expression::getValue() {
    throw "Base class method called.";
}

std::deque<Expression*> Expression::getNumeratorFactors() {
    std::deque<Expression*> dq;
    dq.push_back(this);
    return dq;
}

std::deque<Expression*> Expression::getDenominatorFactors() {
    std::deque<Expression*> dq;
    return dq;
}

std::deque<Expression*> Expression::getFactors() {
    std::deque<Expression*> ret;
    std::deque<Expression*> num = getNumeratorFactors();
    std::deque<Expression*> den = getDenominatorFactors();

    for (Expression* exp : num)
        ret.push_back(exp);
    for (Expression* exp : den)
        ret.push_back(new Division(one, exp));
    return ret;
}

std::deque<Expression*> Expression::getAdditiveTerms() {
    std::deque<Expression*> dq;
    dq.push_back(this);
    return dq;
}

std::string Expression::getString() {
    throw "Base class method called.";
}

bool Expression::isNeg() {
    bool neg = false;
    std::deque<Expression*> terms = getFactors();
    for (Expression* factor : terms)
        if (*factor == *negOne)
            neg = !neg;
    return neg;
}

bool Expression::operator==(Expression& rhs) {
    return getString() == rhs.getString();
}

bool Expression::operator!=(Expression& rhs) {
    return !(rhs == *this);
}

bool Expression::operator<(Expression &rhs) {
    return getString() < rhs.getString();
}

bool OpExpression::operator==(Expression& rhs) {
    OpExpression* rhsExpr = dynamic_cast<OpExpression*>(&rhs);
    if (!rhsExpr)
        return false;

    return *(getLeftSide()) == *(rhsExpr->getLeftSide());
}

std::string OpExpression::getString() {
    std::string left = getLeftSide()->getString();
    if (leftSide->needParenthesis())
        left = "(" + left + ")";

    return left + " " + getOpString();
}

std::string BiOpExpression::getString() {
    std::string right = getRightSide()->getString();
    if (rightSide->needParenthesis())
        right = "(" + right + ")";

    return OpExpression::getString() + " " + right;
}

bool BiOpExpression::operator==(Expression& rhs) {
    BiOpExpression* rhsExpr = dynamic_cast<BiOpExpression*>(&rhs);
    if (!rhsExpr)
        return false;

    return (getOpString() == rhsExpr->getOpString()) && (*(leftSide) == *(rhsExpr->leftSide)) && (*(rightSide) == *(rhsExpr->rightSide));
}