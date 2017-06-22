#ifndef CALC_VARIABLE_H
#define CALC_VARIABLE_H

#include "Expression.h"
#include "Integer.h"
#include "Float.h"
#include <string>
#include <cmath>

class Variable : public Expression{
protected:
    std::string name;
    Expression* value;
public:
    Variable(std::string, Expression*);
    double getValue() override;

    std::string getString() override;
};

static Variable* pi = new Variable("pi", new Double(M_PI));
static Variable* e = new Variable("e", new Double(M_E));
static Variable* i = new Variable("i", new Integer(1));


#endif //CALC_VARIABLE_H
