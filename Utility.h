#ifndef CALC_UTILITY_H
#define CALC_UTILITY_H

#include "Expression.h"
#include "Integer.h"
#include "Multiplication.h"
#include "Addition.h"
#include <deque>
#include <algorithm>
#include <sstream>
#include <string>

Expression* strToExpr(std::string str);
Expression* simplify(Expression* expr);
std::string simplify(std::string str);

Expression* multiplyFactors(std::deque<Expression*> list, bool simplify = false);
Expression* addTerms(std::deque<Expression*> list, bool simplify = false);
std::deque<Expression*> getCommonFactors(std::deque<Expression*> left, std::deque<Expression*> right);

struct ExpressionComp {
    bool operator()(Expression* rhs, Expression* lhs) {
        return (*rhs < *lhs);
    }
};

template <typename T>
T strToT(std::string data) {
    T ret;
    std::istringstream(data) >> ret;
    return ret;
}

template <typename T>
static T mod(T a, T b)
{
    int64_t r = a % b;
    return r < 0 ? r + b : r;
}

template <typename T>
static T mul_mod(T a, T b, T m)
{
    long double x;
    T c;
    if (a >= m) a = mod<T>(a, m);
    if (b >= m) b = mod<T>(b, m);
    x = a;
    c = x * b / m;
    return mod<T>((int64_t)(a * b - c * m), m);
}

template <typename T>
static T pow_mod(T a, T b, T m)
{
    T r = 1;
    while (b > 0) {
        if(b % 2 != 0)
            r = mul_mod<T>(r, a, m);
        b = b >> 1;
        a = mul_mod<T>(a, a, m);
    }
    return mod<T>(r, m);
}

template <typename T> bool PLessComp(T* a, T* b)
{
    return *a < *b;
}

template <typename T> bool PEqualComp(T* a, T* b)
{
    return *a == *b;
}

#endif //CALC_UTILITY_H
