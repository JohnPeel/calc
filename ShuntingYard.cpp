#include "ShuntingYard.h"
#include "Utility.h"
#include "Variable.h"
#include "Method.h"
#include "Division.h"
#include "Exponentiation.h"
#include "Subtraction.h"

#include <stack>
#include <cassert>
#include <iostream>

Expression* tokenToExp(Token tok, std::deque<Expression*>& stack) {
    Expression* right = stack.back(); stack.pop_back();
    Expression* left = stack.back(); stack.pop_back();

    switch (tok.getToken()) {
        case tk_op_Divide: return new Division(left, right);
        case tk_op_Minus: return new Subtraction(left, right);
        case tk_op_Multiply: return new Multiplication(left, right);
        case tk_op_Plus: return new Addition(left, right);
        case tk_op_Power: return new Exponentiation(left, right);

        case tk_op_Rt: //return new NthRoot(left, right);
        case tk_Identifier:
        default:
            throw "What the hell??";
    }
}

void pushOp(Token token, std::deque<Expression *>& output, std::stack<Token>& opers) {
    while ((opers.size() > 0) and ((opers.top().getPrecedence() > token.getPrecedence()) or ((token.getPrecedence() == opers.top().getPrecedence()) and (token.getAssoc() == assocLeft)))) {
        output.push_back(tokenToExp(opers.top(), output));
        opers.pop();
    }

    opers.push(token);
}

ShuntingYard::ShuntingYard(std::string data): parser(data) {
    // stub
}

Expression* ShuntingYard::process() {
    std::deque<Expression *> output;
    std::stack<Token> opers;

    EParserToken lastToken = tk_NULL;
    while (parser.nextTokenNoJunk() != tk_NULL) {
        Token token = parser.getToken();

        switch (token.getToken()) {
            case tk_Identifier:
                if ((lastToken == tk_typ_Integer) or (lastToken == tk_typ_Float) or (lastToken == tk_Identifier))
                    pushOp(Token(tk_op_Multiply, 0, "*"), output, opers);

                Expression* temp;
                if (Method::find(token.getData(), temp))
                    opers.push(token);
                else if (Variable::find(token.getData(), temp))
                    output.push_back(temp);
                else
                    output.push_back(new Variable(token.getData(), NULL));
                break;
            case tk_typ_Integer:
                if ((lastToken == tk_typ_Integer) or (lastToken == tk_typ_Float) or (lastToken == tk_Identifier))
                    pushOp(Token(tk_op_Multiply, 0, "*"), output, opers);

                output.push_back(new Integer(strToT<int>(token.getData())));
                break;
            case tk_typ_Float:
                output.push_back(new Float(strToT<float>(token.getData())));
                break;
            case tk_op_Divide ... tk_op_Rt:
                pushOp(token, output, opers);
                break;
            case tk_sym_ParenthesisOpen: case tk_sym_BracketOpen:
                opers.push(token);
                break;
            case tk_sym_ParenthesisClose:
                while (opers.top().getToken() != tk_sym_ParenthesisOpen) {
                    output.push_back(tokenToExp(opers.top(), output));
                    opers.pop();
                }
                opers.pop();
                break;
            case tk_sym_BracketClose:
                while (opers.top().getToken() != tk_sym_BracketOpen) {
                    output.push_back(tokenToExp(opers.top(), output));
                    opers.pop();
                }
                opers.pop();
                break;
        }

        lastToken = token.getToken();
    }

    while (opers.size() > 0) {
        output.push_back(tokenToExp(opers.top(), output));
        opers.pop();
    }

    if (output.size() > 1) {
        std::cout << "---------------------------------------------------------------" << std::endl;
        std::cout << "Something is wrong... (" << output.size() << ")" << std::endl;
        for (Expression* expr : output)
            std::cout << "\"" << expr->getString() << "\" ";
        std::cout << std::endl << std::endl << "---------------------------------------------------------------" << std::endl;
    }

    return output.front();
}
