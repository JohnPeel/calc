#include <iostream>
#include "Integer.h"
#include "Addition.h"
#include "Subtraction.h"
#include "Multiplication.h"
#include "Division.h"
#include "Variable.h"
#include "Exponentiation.h"
#include "Prime.h"

using namespace std;

int main(void) {
    Expression* expr = new Subtraction(new Multiplication(new Integer(8), new Exponentiation(new Integer(2), new Division(one, new Integer(2)))), new Integer(4));
    cout << "Input: \t\t" << expr->getString() << " = " << expr->getValue() << endl;

    Expression* exprSimp = expr->simplify();
    cout << "Simplified: \t" << exprSimp->getString() << " = " << exprSimp->getValue() << endl;

    return 0;
}