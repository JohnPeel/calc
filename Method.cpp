#include "Method.h"

std::vector<Method*> registeredMethods;

Method::Method(std::string name, void *callback) {
    registeredMethods.push_back(this);
}

bool Method::find(std::string name, Expression *&foundMethod) {
    return false;
}
