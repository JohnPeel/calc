#ifndef CALC_METHOD_H
#define CALC_METHOD_H

#include "Expression.h"

class Method : public Expression {
public:
    Method(std::string name, void* callback);
    static bool find(std::string name, Expression*& foundMethod);
};

#endif //CALC_METHOD_H
