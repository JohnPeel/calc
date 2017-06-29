#ifndef CALC_SHUNTINGYARD_H
#define CALC_SHUNTINGYARD_H

#include <string>
#include <queue>
#include <stack>
#include "Parser.h"
#include "Utility.h"

class ShuntingYard : public Parser {
private:
    ExpressionList output;
    std::stack<Token> opers;
public:
    ShuntingYard(std::string data);

    void Error(std::string) override;
    Expression* tokenToExp(Token tok);
    void pushOp(Token token);
    Expression* process();
};


#endif //CALC_SHUNTINGYARD_H
