#include "Method.h"

MethodList registeredMethod;

Method::Method(std::string name, void *callback) {
    registeredMethod.add(this);
}

bool Method::find(std::string name, Expression*& foundVar) {
    return registeredMethod.find(name, foundVar);
}

void MethodList::add(Method* method) {
    std::string name = method->getString();
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    map[name] = method;
}

bool MethodList::find(std::string name, Expression *&foundVar) {
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    foundVar = map[name];
    return foundVar != NULL;
}
