#include "Variable.h"
#include "Float.h"

#define _USE_MATH_DEFINES // NOTE: Do not delete! Needed by some compilers.
#include <cmath>

VariableList registeredVar;
Expression* pi = new Variable("pi", new Float(M_PI));
Expression* e = new Variable("e", new Float(M_E));
Expression* i = new Variable("i", nullptr);

Variable::Variable(std::string name, Expression* value, bool registerThis) : name(std::move(name)), value(value) {
    if (registerThis)
        registeredVar.add(this);
}

bool Variable::hasValue() {
    return (value != nullptr) && value->hasValue();
}

bool Variable::hasExactValue() {
    return true;
}

double Variable::getValue() {
    return (hasValue()) ? value->getValue() : 0;
}

Expression* Variable::simplify() {
    if ((name != "ans") || (value == nullptr))
        return this;
    return value->simplify();
}

std::string Variable::getString() {
    return ((name == "ans") && (value != nullptr)) ? value->getString() : name;
}

bool Variable::find(std::string name, Expression*& foundVar) {
    return registeredVar.find(std::move(name), foundVar);
}

void VariableList::add(Variable* var) {
    std::string name = var->getString();
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    map[name] = var;
}

bool VariableList::find(std::string name, Expression *&foundVar) {
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    foundVar = map[name];
    return foundVar != nullptr;
}
