#ifndef CALC_UTILITY_H
#define CALC_UTILITY_H

#include "Expression.h"
#include "Integer.h"
#include "Multiplication.h"
#include <vector>
#include <algorithm>

Expression* multiplyFactors(std::vector<Expression*> list);
std::vector<Expression*> getCommonFactors(std::vector<Expression*> left, std::vector<Expression*> right);

template <typename T>
T modpow(T base, T exp, T modulus) {
    base %= modulus;
    T result = 1;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % modulus;
        base = (base * base) % modulus;
        exp >>= 1;
    }
    return result;
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
