#ifndef CALC_METHOD_H
#define CALC_METHOD_H

#include "Utility.h"
#include <string>
#include <utility>

typedef Expression* (*MethodProc)(ExpressionList);

class Method : public Expression {
protected:
    std::string name;
    ExpressionList params;
    MethodProc method;
public:
    Method(std::string, ExpressionList, MethodProc);

    Expression* simplify() override;

    std::string getName() { return name; };
    std::string getTypeString() override { return "Method"; };
    std::string getString() override {
        std::string ret = name + "(";

        for (Expression* param : params)
            ret += param->getString() + ", ";

        return ret.substr(0, ret.length() - 2) + ")";
    }

    static bool find(std::string, int&, MethodProc&);
    static bool find(std::string);
};

class MethodList {
protected:
    std::map<std::string, std::pair<int, MethodProc>> map;
public:
    void add(std::string name, int paramCount, MethodProc method);
    bool find(std::string name, int& paramCount, MethodProc& foundMethod);

    virtual MethodProc operator[](std::string rhs) {
        MethodProc method = NULL;
        int paramCount = 0;
        return (find(rhs, paramCount, method)) ? method : NULL;
    }
};

#endif //CALC_METHOD_H
