#include "Variable.h"

Variable::Variable(std::string name, Expression* value) {
    this->name = name;
    this->value = value->simplify();
}

double Variable::getValue() {
    return value->getValue();
}

std::string Variable::getString() {
    return name;
}
