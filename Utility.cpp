
#include "Utility.h"
#include "Prime.h"

Expression* multiplyFactors(std::vector<Expression*> list) {
    if (list.size() == 0)
        return new Integer(1);

    Expression* ret = list.back();
    list.pop_back();

    while (list.size() > 0) {
        ret = new Multiplication(ret, list.back());
        list.pop_back();
    }

    return ret;
}

std::vector<Expression*> getCommonFactors(std::vector<Expression*> left, std::vector<Expression*> right) {
    std::vector<Expression*> commonFactors;

    std::sort(left.begin(), left.end(), PComp<Expression>);
    std::sort(right.begin(), right.end(), PComp<Expression>);

    std::set_intersection(left.begin(), left.end(), right.begin(), right.end(), std::back_inserter(commonFactors), PComp<Expression>);

    return commonFactors;
}