#ifndef CALC_SHUNTINGYARD_H
#define CALC_SHUNTINGYARD_H

#include <string>
#include <queue>
#include "Parser.h"
#include "Expression.h"

class ShuntingYard {
private:
    Parser parser;
public:
    ShuntingYard(std::string data);
    Expression* process();
};


#endif //CALC_SHUNTINGYARD_H
