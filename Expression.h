#ifndef CALC_EXPRESSION_H
#define CALC_EXPRESSION_H

#include <vector>
#include <string>

class Expression {
public:
    virtual bool hasValue();
    virtual double getValue();

    virtual Expression* getLeftSide();
    virtual Expression* getRightSide();

    virtual std::vector<Expression*> getNumeratorFactors();
    virtual std::vector<Expression*> getDenominatorFactors();

    virtual std::vector<Expression*> getAdditiveTerms();

    virtual Expression* simplify();
    virtual std::string getString();

    bool operator==(Expression& rhs);
    bool operator!=(Expression& rhs);
    bool operator<(Expression& rhs);
};

#endif //CALC_EXPRESSION_H
