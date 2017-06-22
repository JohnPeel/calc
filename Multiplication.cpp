#include "Multiplication.h"
#include "Integer.h"
#include "Variable.h"
#include "Utility.h"

Multiplication::Multiplication(Expression* leftSide, Expression* rightSide) {
    this->leftSide = leftSide;
    this->rightSide = rightSide;
}

double Multiplication::getValue() {
    return leftSide->getValue() * rightSide->getValue();
}

Expression* Multiplication::getLeftSide() {
    return leftSide;
}

Expression* Multiplication::getRightSide() {
    return rightSide;
}

std::vector<Expression *> Multiplication::getNumeratorFactors() {
    std::vector<Expression*> vect;

    Expression* leftSide = this->leftSide->simplify();
    Expression* rightSide = this->rightSide->simplify();

    std::vector<Expression*> _temp = leftSide->getNumeratorFactors();
    vect.insert(vect.end(), _temp.begin(), _temp.end());

    _temp = rightSide->getNumeratorFactors();
    vect.insert(vect.end(), _temp.begin(), _temp.end());

    return vect;
}

Expression* Multiplication::simplify() {
    return commutativeSimplify<Multiplication>(this, leftSide->simplify(), rightSide->simplify());
}

std::string Multiplication::getString() {
    Integer* leftSideInt = dynamic_cast<Integer*>(leftSide);
    Integer* rightSideInt = dynamic_cast<Integer*>(rightSide);
    Variable* rightSideVar = dynamic_cast<Variable*>(rightSide);
    Variable* leftSideVar = dynamic_cast<Variable*>(leftSide);

    if (leftSideInt and (not rightSideInt))
        if (rightSideVar)
            return leftSideInt->getString() + rightSideVar->getString();
        else
            return leftSideInt->getString() + "(" + rightSide->getString() + ")";

    if (rightSideInt and (not leftSideInt))
        if (leftSideVar)
            return rightSideInt->getString() + leftSideVar->getString();
        else
            return rightSideInt->getString() + "(" + leftSide->getString() + ")";

    return "(" + leftSide->getString() + ")(" + rightSide->getString() + ")";
}
