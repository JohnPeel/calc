#include <algorithm>
#include <functional>
#include "Method.h"
#include "Variable.h"
#include "Integer.h"
#include "Division.h"
#include "Exponentiation.h"

MethodList registeredMethod;

class initMethods {
public:
    initMethods() {
        registeredMethod.add("simplify", 1, [](ExpressionList params) -> Expression* {
            return params[0]->simplify();
        });

        registeredMethod.add("factor", 1, [](ExpressionList params) -> Expression* {
            return multiplyFactors(params[0]->getFactors());
        });

        registeredMethod.add("type", 1, [](ExpressionList params) -> Expression* {
            return new Variable(params[0]->getTypeString(), NULL, false);
        });

        /* // FIXME: Need to finish writing this.
        registeredMethod.add("sin", 1, [](ExpressionList params) -> Expression* {
            Expression* angle = params[0]->simplify();

            if (angle->getString() == "pi / 2")
                return new Division(one, new NthRoot(two, two));

            ExpressionList newParams; newParams.push_back(angle);
            return new Method("sin", newParams, registeredMethod["sin"]);
        });
        */
    }
};

initMethods methods;

Method::Method(std::string name, ExpressionList params, MethodProc method) {
    this->name = name;
    this->params = params;
    this->method = method;
}

Expression* Method::simplify() {
    if (!method)
        return this;

    Expression* expr = method(params);
    if (expr)
        return expr;

    return this;
}

bool Method::find(std::string name, int& paramCount, MethodProc& method) {
    return registeredMethod.find(name, paramCount, method);
}

bool Method::find(std::string name) {
    int paramCount;
    MethodProc method;
    return registeredMethod.find(name, paramCount, method);
}

void MethodList::add(std::string name, int paramCount, MethodProc method) {
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    map[name] = std::make_pair(paramCount, method);
}

bool MethodList::find(std::string name, int& paramCount, MethodProc& foundMethod) {
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    auto foundPair = map[name];
    paramCount = foundPair.first;
    foundMethod = foundPair.second;
    return foundMethod != NULL;
}
