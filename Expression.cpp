#include "Expression.h"
#include "Integer.h"

Expression* Expression::simplify() {
    return this;
}


bool Expression::hasValue() {
    return false;
}

double Expression::getValue() {
    throw "Base class method called.";
}

Expression* Expression::getLeftSide() {
    throw "Base class method called.";
}

Expression* Expression::getRightSide() {
    throw "Base class method called.";
}

std::vector<Expression*> Expression::getNumeratorFactors() {
    std::vector<Expression*> vect;
    vect.push_back(this);
    return vect;
}

std::vector<Expression*> Expression::getDenominatorFactors() {
    std::vector<Expression*> vect;
    vect.push_back(one);
    return vect;
}

std::vector<Expression*> Expression::getAdditiveTerms() {
    std::vector<Expression*> vect;
    vect.push_back(this);
    return vect;
}

std::string Expression::getString() {
    throw "Base class method called.";
}

bool Expression::operator==(Expression& rhs) {
    return getValue() == rhs.getValue();
}

bool Expression::operator!=(Expression& rhs) {
    return !(rhs == *this);
}

bool Expression::operator<(Expression &rhs) {
    return getValue() < rhs.getValue();
}
