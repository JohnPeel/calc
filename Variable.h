#ifndef CALC_VARIABLE_H
#define CALC_VARIABLE_H

#define _USE_MATH_DEFINES
#include "Expression.h"
#include "Integer.h"
#include "Float.h"
#include <map>
#include <string>
#include <cmath>
#include <algorithm>

class Variable : public Expression {
protected:
    std::string name;
    Expression* value;
public:
    Variable(std::string, Expression*);
    double getValue() override;
    bool hasValue() override;
    bool hasExactValue() override;

    std::string getString() override;

    static bool find(std::string name, Expression*& foundVar);

    bool operator==(Expression& rhs) override { return this->getString() == rhs.getString(); }
};

class VariableList {
protected:
    std::map<std::string, Variable*> map;
public:
    void add(Variable* var);
    bool find(std::string name, Expression*& foundVar);
};

extern Variable* pi;
extern Variable* e;
extern Variable* i;

#endif //CALC_VARIABLE_H
