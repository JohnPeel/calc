#include "Variable.h"
#include "Float.h"

#define _USE_MATH_DEFINES // NOTE: Do not delete! Needed by some compilers.
#include <cmath>

VariableList registeredVar;
Expression* pi = new Variable("pi", new Float(M_PI));
Expression* e = new Variable("e", new Float(M_E));
Expression* i = new Variable("i", NULL);

Variable::Variable(std::string name, Expression* value, bool registerThis) {
    this->name = name;
    this->value = value;

    if (registerThis)
        registeredVar.add(this);
}

bool Variable::hasValue() {
    return (value != NULL) && value->hasValue();
}

bool Variable::hasExactValue() {
    return true;
}

double Variable::getValue() {
    return (hasValue()) ? value->getValue() : 0;
}

std::string Variable::getString() {
    return (name == "ans") ? value->getString() : name;
}

bool Variable::find(std::string name, Expression*& foundVar) {
    return registeredVar.find(name, foundVar);
}

void VariableList::add(Variable* var) {
    std::string name = var->getString();
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    map[name] = var;
}

bool VariableList::find(std::string name, Expression *&foundVar) {
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    foundVar = map[name];
    return foundVar != NULL;
}
