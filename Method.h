#ifndef CALC_METHOD_H
#define CALC_METHOD_H

#include "Utility.h"
#include <string>

typedef Expression* (*MethodProc)(ExpressionList);

class Method : public Expression {
protected:
    std::string name;
    int paramCount;
    MethodProc method;
public:
    Method(std::string, int, MethodProc, bool = true);

    Expression* call(ExpressionList params);

    std::string getName() { return name; };
    std::string getTypeString() override { return "Method"; };

    int getParamCount();

    static bool find(std::string name, Expression*& foundMethod);
};

class MethodList {
protected:
    std::map<std::string, Method*> map;
public:
    void add(Method* var);
    bool find(std::string name, Expression*& foundVar);
};

extern Method* methodFactor;
extern Method* methodSimplify;
extern Method* methodType;

#endif //CALC_METHOD_H
