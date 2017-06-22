#ifndef CALC_UTILITY_H
#define CALC_UTILITY_H

#include "Expression.h"
#include "Integer.h"
#include "Multiplication.h"
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>

Expression* multiplyFactors(std::vector<Expression*> list);
std::vector<Expression*> getCommonFactors(std::vector<Expression*> left, std::vector<Expression*> right);

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
    int64_t r;
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

template <typename T> bool PComp(T* a, T* b)
{
    return *a < *b;
}

template <typename T> Expression* commutativeSimplify(T* self, Expression* leftSide, Expression* rightSide) {
    Integer* leftSideInt = dynamic_cast<Integer*>(leftSide);
    Integer* rightSideInt = dynamic_cast<Integer*>(rightSide);

    if (rightSideInt and leftSideInt)
        return new Integer(self->getValue());

    T* leftSideT = dynamic_cast<T*>(leftSide);
    T* rightSideT = dynamic_cast<T*>(rightSide);

    if (rightSideT and leftSideInt) {
        Integer* rightSideTR = dynamic_cast<Integer*>(rightSideT->getRightSide());
        if (rightSideTR) {
            T* _temp = new T(rightSideTR, leftSideInt);
            return new T(_temp->simplify(), rightSideT->getLeftSide());
        }

        Integer* rightSideTL = dynamic_cast<Integer*>(rightSideT->getLeftSide());
        if (rightSideTL) {
            T* _temp = new T(rightSideTL, leftSideInt);
            return new T(_temp->simplify(), rightSideT->getRightSide());
        }
    }

    if (leftSideT and rightSideInt) {
        Integer* leftSideTR = dynamic_cast<Integer*>(leftSideT->getRightSide());
        if (leftSideTR) {
            T* _temp = new T(leftSideTR, rightSideInt);
            return new T(_temp->simplify(), leftSideT->getLeftSide());
        }

        Integer* leftSideTL = dynamic_cast<Integer*>(leftSideT->getLeftSide());
        if (leftSideTL) {
            T* _temp = new T(leftSideTL, rightSideInt);
            return new T(_temp->simplify(), leftSideT->getRightSide());
        }
    }

    if (rightSideT and leftSideT) {
        Integer* rightSideTR = dynamic_cast<Integer*>(rightSideT->getRightSide());
        Integer* rightSideTL = dynamic_cast<Integer*>(rightSideT->getLeftSide());
        Integer* leftSideTR = dynamic_cast<Integer*>(leftSideT->getRightSide());
        Integer* leftSideTL = dynamic_cast<Integer*>(leftSideT->getLeftSide());

        if (rightSideTR and leftSideTR) {
            T* _temp = new T(rightSideTR, leftSideTR);
            return new T(_temp->simplify(), new T(rightSideT->getLeftSide(), leftSideT->getLeftSide()));
        }

        if (rightSideTR and leftSideTL) {
            T* _temp = new T(rightSideTR, leftSideTL);
            return new T(_temp->simplify(), new T(rightSideT->getLeftSide(), leftSideT->getRightSide()));
        }

        if (rightSideTL and leftSideTR) {
            T* _temp = new T(rightSideTL, leftSideTR);
            return new T(_temp->simplify(), new T(rightSideT->getRightSide(), leftSideT->getLeftSide()));
        }

        if (rightSideTL and leftSideTL) {
            T* _temp = new T(rightSideTL, leftSideTL);
            return new T(_temp->simplify(), new T(rightSideT->getRightSide(), leftSideT->getRightSide()));
        }
    }

    return new T(leftSide, rightSide);
}

#endif //CALC_UTILITY_H
