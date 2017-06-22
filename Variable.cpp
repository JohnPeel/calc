#include "Variable.h"

std::vector<Variable*> registeredVar;

Variable::Variable(std::string name, Expression* value) {
    this->name = name;
    if (value)
        this->value = value->simplify();
    else
        this->value = NULL;

    registeredVar.push_back(this);
}

bool Variable::hasValue() {
    return (value != NULL) and value->hasValue();
}


double Variable::getValue() {
    return (hasValue()) ? value->getValue() : 0;
}

std::string Variable::getString() {
    return name;
}


bool Variable::find(std::string name, Expression *&foundVar) {
    for (Variable* var : registeredVar) {
        if (var->getString() == name) {
            foundVar = var;
            return true;
        }
    }
    return false;
}
