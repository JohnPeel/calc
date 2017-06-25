#ifndef CALC_LOGARITHM_H
#define CALC_LOGARITHM_H

#include "Expression.h"

class Logarithm : public BiOpExpression {
public:
    Logarithm(Expression* left, Expression* right) : BiOpExpression(left, right) {};
    std::string getOpString() { return "log"; };
};


#endif //CALC_LOGARITHM_H
