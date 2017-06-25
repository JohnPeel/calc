
#include "Utility.h"
#include "Prime.h"
#include "ShuntingYard.h"

Expression* strToExpr(std::string str) {
    return ShuntingYard(str).process();
}

Expression* simplify(Expression* expr) {
    return expr->simplify();
}

std::string simplify(std::string str) {
    return strToExpr(str)->simplify()->getString();
}

Expression* multiplyFactors(std::deque<Expression*> list, bool simplify) {
    if (list.size() == 0)
        return new Integer(1);

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

Expression* addTerms(std::deque<Expression*> list, bool simplify) {
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

std::deque<Expression*> getCommonFactors(std::deque<Expression*> left, std::deque<Expression*> right) {
    std::deque<Expression*> commonFactors;

    for (Expression* item : left)
        for (Expression* compare : right)
            if (*item == *compare) {
                commonFactors.push_back(item);
                right.erase(std::find(right.begin(), right.end(), compare));
                break;
            }

    return commonFactors;
}