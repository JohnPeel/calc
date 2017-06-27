#ifndef CALC_SHUNTINGYARD_H
#define CALC_SHUNTINGYARD_H

#include <string>
#include <queue>
#include "Parser.h"
#include "Expression.h"

class ShuntingYard : public Parser {
public:
    ShuntingYard(std::string data);

    void Error(std::string) override;
    Expression* process();
};


#endif //CALC_SHUNTINGYARD_H
