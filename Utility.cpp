
#include <cmath>
#include "Utility.h"
#include "Prime.h"
#include "ShuntingYard.h"
#include "Integer.h"
#include "Addition.h"
#include "Multiplication.h"
#include "Division.h"

Expression* strToExpr(std::string str) {
    return ShuntingYard(str).process();
}

Expression* simplify(Expression* expr) {
    return expr->simplify();
}

std::string simplify(std::string str) {
    return strToExpr(str)->simplify()->getString();
}

ExpressionList factorMapToList(ExpressionMap factorMap) {
    ExpressionList terms;
    for (auto x : factorMap)
        while (factorMap[x.first] != 0) {
            if (factorMap[x.first] > 0) {
                terms.push_back(x.first);
                factorMap[x.first]--;
            } else {
                terms.push_back(new Division(one, x.first));
                factorMap[x.first]++;
            }
        }

    return terms;
}

ExpressionMap listToFactorMap(ExpressionList deque) {
    ExpressionMap factorMap;
    for (Expression* factor : deque)
        if (*factor != *one)
            factorMap[factor]++;
    return factorMap;
}

Expression* multiplyFactors(ExpressionList list, bool simplify) {
    if (list.size() == 0)
        return one;

    Expression* ret = (simplify) ? list.front()->simplify() : list.front();
    list.pop_front();

    while (list.size() > 0) {
        Expression* term = (simplify) ? list.front()->simplify() : list.front();
        list.pop_front();

        if (*term != *one)
            ret = new Multiplication(ret, term);
    }

    return ret;
}

Expression* addTerms(ExpressionList list, bool simplify) {
    if (list.size() == 0)
        return new Integer(0);

    Expression* ret = (simplify) ? list.front()->simplify() : list.front();
    list.pop_front();

    while (list.size() > 0) {
        Expression* term = (simplify) ? list.front()->simplify() : list.front();
        list.pop_front();

        if (*term != *zero)
            ret = new Addition(ret, term);
    }

    return ret;
}

ExpressionList getCommonFactors(ExpressionList left, ExpressionList right) {
    ExpressionList commonFactors;

    for (Expression* item : left)
        for (Expression* compare : right)
            if (*item == *compare) {
                commonFactors.push_back(item);
                right.erase(std::find(right.begin(), right.end(), compare));
                break;
            }

    return commonFactors;
}

unsigned int richards(double const x0, unsigned long long& num, unsigned long long& den, double const error) {
    double g = std::abs(x0), x_abs = g;
    unsigned long long a = 0, b = 1, c = 1, d = 0, s;
    unsigned int iter = 0;

    do {
        s = (unsigned long long)std::floor(g);

        num = a + (s * c);
        den = b + (s * d);
        a = c;
        b = d;
        c = num;
        d = den;
        g = 1.0 / (g - s);

        if (std::abs((double)num / den - x_abs) < error)
            return iter;
    } while (++iter < 100);

    return 0;
}
