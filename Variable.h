#ifndef CALC_VARIABLE_H
#define CALC_VARIABLE_H

#include "Expression.h"
#include <map>
#include <algorithm>

class Variable : public Expression {
protected:
    std::string name;
    Expression* value;
public:
    Variable(std::string, Expression*, bool = true);
    double getValue() override;
    bool hasValue() override;
    bool hasExactValue() override;

    Expression* simplify() override;

    std::string getString() override;
    std::string getTypeString() override { return "Variable"; };

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

extern Expression* pi;
extern Expression* e;
extern Expression* i;

#endif //CALC_VARIABLE_H
