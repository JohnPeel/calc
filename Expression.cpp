#include "Utility.h"
#include "Integer.h"
#include "Variable.h"
#include "Exponentiation.h"

double Expression::getValue() {
    throw "Base class method called.";
}

ExpressionList Expression::getNumeratorFactors() {
    ExpressionList dq;
    dq.push_back(this);
    return dq;
}

ExpressionList Expression::getDenominatorFactors() {
    ExpressionList dq;
    return dq;
}

ExpressionList Expression::getFactors() {
    ExpressionList ret = getNumeratorFactors();
    ExpressionList den = getDenominatorFactors();

    for (Expression* exp : den)
        ret.push_back(new Exponentiation(exp, negOne));

    return ret;
}

ExpressionList Expression::getAdditiveTerms() {
    ExpressionList dq;
    dq.push_back(this);
    return dq;
}

std::string Expression::getString() {
    throw "Base class method called.";
}

bool Expression::isNeg() {
    bool neg = false;
    ExpressionList terms = getFactors();
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