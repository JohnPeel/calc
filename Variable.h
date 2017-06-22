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
    bool hasValue() override;

    std::string getString() override;

    static bool find(std::string name, Expression*& foundVar);
};

static Variable* pi = new Variable("pi", new Double(M_PI));
static Variable* e = new Variable("e", new Double(M_E));
static Variable* i = new Variable("i", NULL);


#endif //CALC_VARIABLE_H
