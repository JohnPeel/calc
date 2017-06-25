#ifndef CALC_METHOD_H
#define CALC_METHOD_H

#include "Expression.h"
#include <map>
#include <string>
#include <algorithm>

class Method : public Expression {
public:
    Method(std::string name, void* callback);
    static bool find(std::string name, Expression*& foundMethod);
};

class MethodList {
protected:
    std::map<std::string, Method*> map;
public:
    void add(Method* var);
    bool find(std::string name, Expression*& foundVar);
};

#endif //CALC_METHOD_H
