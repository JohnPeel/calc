#ifndef CALC_EXPRESSION_H
#define CALC_EXPRESSION_H

#include <deque>
#include <string>

class Expression {
public:
    Expression() {};

    virtual double getValue();
    virtual bool hasValue() { return false; };
    virtual bool hasExactValue() { return false; };

    virtual std::deque<Expression*> getNumeratorFactors();
    virtual std::deque<Expression*> getDenominatorFactors();
    virtual std::deque<Expression*> getFactors();

    virtual std::deque<Expression*> getAdditiveTerms();

    //virtual Expression* factor() { return this; };
    //virtual Expression* expand() { return this; };
    virtual Expression* simplify() { return this; };

    virtual bool needParenthesis() { return (getNumeratorFactors().size() + getDenominatorFactors().size() > 1) or (getAdditiveTerms().size() > 1); };
    virtual std::string getString();

    virtual bool isNeg();

    virtual bool operator==(Expression& rhs);
    virtual bool operator!=(Expression& rhs);
    virtual bool operator<(Expression& rhs);
};

class OpExpression : public Expression {
protected:
    Expression* leftSide;
public:
    OpExpression(Expression* left) : leftSide(left) {};

    bool hasValue() override { return getLeftSide()->hasValue(); };
    bool hasExactValue() override { return getLeftSide()->hasExactValue(); };

    bool needParenthesis() override { return false; };
    std::string getString() override;
    virtual std::string getOpString() = 0;

    virtual Expression* getLeftSide() { return leftSide; };

    bool operator==(Expression& rhs) override;
};

class BiOpExpression : public OpExpression {
protected:
    Expression* rightSide;
public:
    BiOpExpression(Expression* left, Expression* right) : OpExpression(left), rightSide(right) {};

    bool hasValue() override { return getLeftSide()->hasValue() and getRightSide()->hasValue(); };
    bool hasExactValue() override { return getLeftSide()->hasExactValue() and getRightSide()->hasExactValue(); };

    bool needParenthesis() override { return true; };
    std::string getString() override;

    virtual Expression* getRightSide() { return rightSide; };

    bool operator==(Expression& rhs) override;
};

#endif //CALC_EXPRESSION_H
