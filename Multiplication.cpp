#include "Multiplication.h"
#include "Integer.h"
#include "Variable.h"
#include "Addition.h"
#include "Division.h"
#include "Exponentiation.h"

double Multiplication::getValue() {
    return leftSide->getValue() * rightSide->getValue();
}

ExpressionList Multiplication::getNumeratorFactors() {
    ExpressionList vect = leftSide->simplify()->getNumeratorFactors();
    ExpressionList _temp = rightSide->simplify()->getNumeratorFactors();
    vect.insert(vect.end(), _temp.begin(), _temp.end());
    return vect;
}

ExpressionList Multiplication::getDenominatorFactors() {
    ExpressionList vect = leftSide->simplify()->getDenominatorFactors();
    ExpressionList _temp = rightSide->simplify()->getDenominatorFactors();
    vect.insert(vect.end(), _temp.begin(), _temp.end());
    return vect;
}

Expression* Multiplication::simplify() {
    Expression* left = leftSide->simplify();
    Expression* right = rightSide->simplify();
    Integer* leftInt = dynamic_cast<Integer*>(left);
    Integer* rightInt = dynamic_cast<Integer*>(right);

    if ((*left == *i) && (*right == *i)) return negOne;
    if (leftInt && rightInt) return new Integer((int)getValue());
    if ((leftInt) && (*leftInt == *one)) return right;
    if ((rightInt) && (*rightInt == *one)) return left;

    ExpressionList terms = getNumeratorFactors();

    if (terms.size() > 1) {
        ExpressionMap factorMap = listToFactorMap(getNumeratorFactors());

        factorMap[one] = 0;
        factorMap[i] %= 4;
        switch (factorMap[i]) {
            case 2:
                factorMap[negOne] += 1;
                factorMap.erase(i);
                break;
            case 3:
                factorMap[negOne] += 1;
                factorMap[i] = 1;
                break;
            default:
                break;
        }
        factorMap[negOne] %= 2;

        ExpressionList intTerms;
        for (auto& term : factorMap)
                if (term.second != 0) {
                    if (dynamic_cast<Integer *>(term.first) != NULL) {
                        intTerms.push_back(new Exponentiation(term.first, new Integer(term.second)));
                        factorMap.erase(term.first);
                    } else if (term.second > 1) {
                        factorMap[new Exponentiation(term.first, new Integer(term.second))] = 1;
                        factorMap.erase(term.first);
                    }
                }

        Expression* intTerm = multiplyFactors(intTerms)->simplify();
        if (*intTerm != *one)
            factorMap[intTerm] = 1;

        terms = factorMapToList(factorMap);
    }

    if (terms.size() == 2) {
        Expression* outsideTerm = dynamic_cast<Integer*>(terms[0]);
        if (!outsideTerm) outsideTerm = dynamic_cast<Integer*>(terms[1]);
        if (!outsideTerm) outsideTerm = dynamic_cast<Variable*>(terms[0]);
        if (!outsideTerm) outsideTerm = dynamic_cast<Variable*>(terms[1]);

        Addition* addTerm = dynamic_cast<Addition*>(terms[0]);
        if (!addTerm) addTerm = dynamic_cast<Addition*>(terms[1]);

        if (outsideTerm && addTerm) {
            ExpressionList termList;
            for (Expression* term : addTerm->getAdditiveTerms())
                termList.push_back((new Multiplication(outsideTerm, term))->simplify());

            if ((termList.size() > 1) && (termList[0]->isNeg()))
                for (size_t i = 1; i < termList.size(); i++)
                    if (!termList[i]->isNeg())
                        std::swap(termList[0], termList[i]); // Causes a warning? but works...

            terms.clear();
            terms.push_back(addTerms(termList));
        }
    }

    ExpressionList denFactors = getDenominatorFactors();

    Expression* expr;
    if ((denFactors.size() == 0) || ((denFactors.size() == 1) && (*(denFactors.front()) == *one))) {
        if (terms.size() > 1)
            expr = multiplyFactors(terms);
        else
            expr = terms.front();
    } else
        expr = (new Division(multiplyFactors(terms), multiplyFactors(denFactors)))->simplify();

    return expr;
}

bool Multiplication::needParenthesis() {
    if (leftSide->needParenthesis() && (!rightSide->needParenthesis()))
        return false;

    if (rightSide->needParenthesis() && (!leftSide->needParenthesis()))
        return false;

    if ((!leftSide->needParenthesis()) && (!rightSide->needParenthesis())) {
        Variable* leftVar = dynamic_cast<Variable*>(leftSide);
        Variable* rightVar = dynamic_cast<Variable*>(rightSide);

        return ((leftVar != NULL) == (rightVar != NULL));
    }

    return Expression::needParenthesis();
}

std::string Multiplication::getString() {
    ExpressionMap map = listToFactorMap(getFactors());
    if (map.size() > 2) {
        std::string ret;

        for (auto factor : map)
            if (*(factor.first) != *one) {
                if (factor.second == 1)
                    ret += "(" + factor.first->getString() + ")";
                else if (factor.second > 1)
                    ret += "(" + (new Exponentiation(factor.first, new Integer(factor.second)))->getString() + ")";
            }

        return ret;
    }

    std::string left = getLeftSide()->getString();
    std::string right = getRightSide()->getString();

    if (leftSide->needParenthesis()) {
        left = "(" + left + ")";
        if (!rightSide->needParenthesis())
            return right + left;
    }

    if (rightSide->needParenthesis()) {
        right = "(" + right + ")";
        return left + right;
    }

    if ((!leftSide->needParenthesis()) && (!rightSide->needParenthesis())) {
        Variable* leftVar = dynamic_cast<Variable*>(leftSide);
        Variable* rightVar = dynamic_cast<Variable*>(rightSide);

        if (leftVar && (!rightVar))
            return right + left;
        if (rightVar && (!leftVar))
            return left + right;
    }

    return left + " " + getOpString() + " " + right;
}
