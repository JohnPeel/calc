#include "Multiplication.h"
#include "Integer.h"
#include "Variable.h"
#include "Utility.h"
#include "Division.h"
#include "Exponentiation.h"

double Multiplication::getValue() {
    return leftSide->getValue() * rightSide->getValue();
}

std::deque<Expression *> Multiplication::getNumeratorFactors() {
    std::deque<Expression*> vect;

    Expression* leftSide = this->leftSide->simplify();
    Expression* rightSide = this->rightSide->simplify();

    std::deque<Expression*> _temp = leftSide->getNumeratorFactors();
    vect.insert(vect.end(), _temp.begin(), _temp.end());

    _temp = rightSide->getNumeratorFactors();
    vect.insert(vect.end(), _temp.begin(), _temp.end());

    return vect;
}

Expression* Multiplication::simplify() {
    Expression* left = leftSide->simplify();
    Expression* right = rightSide->simplify();
    Integer* leftInt = dynamic_cast<Integer*>(left);
    Integer* rightInt = dynamic_cast<Integer*>(right);

    if ((*left == *i) and (*right == *i)) return negOne;
    if (leftInt and rightInt) return new Integer((int)getValue());
    if ((leftInt) and (*leftInt == *one)) return right;
    if ((rightInt) and (*rightInt == *one)) return left;

    std::deque<Expression*> terms = getNumeratorFactors();

    if (terms.size() > 1) {
        std::map<Expression*, int, ExpressionComp> factorMap;
        factorMap[i] = 0;
        factorMap[negOne] = 0;
        for (Expression* term : getNumeratorFactors())
            if (*term != *one) {
                term = term->simplify();
                Exponentiation* termExp = dynamic_cast<Exponentiation*>(term);

                if (termExp) {
                    Integer* termRight = dynamic_cast<Integer*>(termExp->getRightSide());
                    if (termRight)
                        factorMap[termExp->getLeftSide()] += termRight->getValue();
                    else
                        factorMap[term] += 1;
                } else
                    factorMap[term] += 1;
            }

        factorMap[i] %= 4;
        switch (factorMap[i]) {
            case 2:
                factorMap[negOne] += 1;
                factorMap[i] = 0;
                break;
            case 3:
                factorMap[negOne] += 1;
                factorMap[i] = 1;
                break;
            default:
                break;
        }
        factorMap[negOne] %= 2;

        std::deque<Expression*> allTerms;
        for (auto& term : factorMap)
            if (*(term.first) != *one) {
                if ((term.second > 1) and (dynamic_cast<Integer*>(term.first) == NULL))
                    allTerms.push_back(new Exponentiation(term.first, new Integer(term.second)));
                else if (term.second != 0)
                    for (int i = 0; i < term.second; i++)
                        allTerms.push_back(term.first);
            }

        terms.clear();
        std::deque<Expression*> intTerms;
        std::remove_copy_if(allTerms.begin(), allTerms.end(), std::back_inserter(terms), [](Expression* e){ return (dynamic_cast<Integer*>(e) != NULL); });
        std::remove_copy_if(allTerms.begin(), allTerms.end(), std::back_inserter(intTerms), [](Expression* e){ return (dynamic_cast<Integer*>(e) == NULL); });

        if (intTerms.size() > 0)
            terms.push_back(multiplyFactors(intTerms)->simplify());
    }

    if (terms.size() == 2) {
        Expression* outsideTerm = dynamic_cast<Integer*>(terms[0]);
        if (not outsideTerm) outsideTerm = dynamic_cast<Integer*>(terms[1]);
        if (not outsideTerm) outsideTerm = dynamic_cast<Variable*>(terms[0]);
        if (not outsideTerm) outsideTerm = dynamic_cast<Variable*>(terms[1]);

        Addition* addTerm = dynamic_cast<Addition*>(terms[0]);
        if (not addTerm) addTerm = dynamic_cast<Addition*>(terms[1]);

        if (outsideTerm and addTerm) {
            std::deque<Expression*> termList;
            for (Expression* term : addTerm->getAdditiveTerms())
                termList.push_back((new Multiplication(outsideTerm, term))->simplify());

            if ((termList.size() > 1) and (termList[0]->isNeg()))
                for (int i = 1; i < termList.size(); i++)
                    if (not termList[i]->isNeg())
                        std::swap(termList[0], termList[i]);

            terms.clear();
            terms.push_back(addTerms(termList));
        }
    }

    std::deque<Expression*> denFactors = getDenominatorFactors();

    Expression* expr;
    if ((denFactors.size() == 0) or ((denFactors.size() == 1) and (*(denFactors.front()) == *one))) {
        if (terms.size() > 1)
            expr = multiplyFactors(terms);
        else
            expr = terms.front();
    } else
        expr = (new Division(multiplyFactors(terms), multiplyFactors(denFactors)))->simplify();

    return expr;
}

bool Multiplication::needParenthesis() {
    if (leftSide->needParenthesis() and (not rightSide->needParenthesis()))
        return false;

    if (rightSide->needParenthesis() and (not leftSide->needParenthesis()))
        return false;

    if ((not leftSide->needParenthesis()) and (not rightSide->needParenthesis())) {
        Variable* leftVar = dynamic_cast<Variable*>(leftSide);
        Variable* rightVar = dynamic_cast<Variable*>(rightSide);

        return not ((leftVar and (not rightVar)) or (rightVar and (not leftVar)));
    }

    return Expression::needParenthesis();
}

std::string Multiplication::getString() {
    std::string left = getLeftSide()->getString();
    std::string right = getRightSide()->getString();

    if (leftSide->needParenthesis()) {
        left = "(" + left + ")";
        if (not rightSide->needParenthesis())
            return right + left;
    }

    if (rightSide->needParenthesis()) {
        right = "(" + right + ")";
        return left + right;
    }

    if ((not leftSide->needParenthesis()) and (not rightSide->needParenthesis())) {
        Variable* leftVar = dynamic_cast<Variable*>(leftSide);
        Variable* rightVar = dynamic_cast<Variable*>(rightSide);

        if (leftVar and (not rightVar))
            return right + left;
        if (rightVar and (not leftVar))
            return left + right;
    }

    return left + " " + getOpString() + " " + right;
}
