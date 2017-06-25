#ifndef CALC_FLOAT_H
#define CALC_FLOAT_H

#include "Expression.h"

class Float : public Expression {
protected:
    double value;
public:
    Float(double);
    double getValue() override;
    bool hasValue() override { return true; };

    Expression* simplify() override;

    std::string getString() override;
};

#endif //CALC_FLOAT_H
