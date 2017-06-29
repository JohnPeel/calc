#include <algorithm>
#include "Method.h"
#include "Variable.h"

MethodList registeredMethod;

Method* methodSimplify = new Method("simplify", 1, [](ExpressionList params) -> Expression* {
    return params[0]->simplify();
});

Method* methodFactor = new Method("factor", 1, [](ExpressionList params) -> Expression* {
    return multiplyFactors(params[0]->getFactors());
});

Method* methodType = new Method("type", 1, [](ExpressionList params) -> Expression* {
    return new Variable(params[0]->getTypeString(), NULL, false);
});

Method::Method(std::string name, int paramCount, MethodProc method, bool registerThis) {
    this->name = name;
    this->paramCount = paramCount;
    this->method = method;

    if (registerThis)
        registeredMethod.add(this);
}

Expression* Method::call(ExpressionList params) {
    if ((int)params.size() != paramCount)
        throw "Incorrect number of params to " + name + ".";

    if (!method)
        return this;

    Expression* expr = method(params);
    if (expr)
        return expr;

    return this;
}

int Method::getParamCount() {
    return paramCount;
}

bool Method::find(std::string name, Expression*& foundVar) {
    return registeredMethod.find(name, foundVar);
}

void MethodList::add(Method* method) {
    std::string name = method->getName();
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    map[name] = method;
}

bool MethodList::find(std::string name, Expression*& foundVar) {
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    foundVar = map[name];
    return foundVar != NULL;
}
