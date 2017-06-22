#ifndef CALC_FLOAT_H
#define CALC_FLOAT_H

#include "Expression.h"

class Float : public Expression {
protected:
    float value;
public:
    Float(float);
    double getValue() override;

    std::string getString() override;
};

class Double : public Expression {
protected:
    double value;
public:
    Double(double);
    double getValue() override;

    std::string getString() override;
};

#endif //CALC_FLOAT_H
