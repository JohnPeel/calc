#include <set>
#include "ShuntingYard.h"
#include "Float.h"
#include "Variable.h"
#include "Method.h"
#include "Division.h"
#include "Exponentiation.h"
#include "Subtraction.h"
#include "Logarithm.h"

std::set<EParserToken> TwoSidedOps = {tk_op_Divide, tk_op_Minus, tk_op_Multiply, tk_op_Plus, tk_op_Power, tk_op_Rt, tk_op_Log};
std::set<EParserToken> OneSidedOps = {tk_op_Ln};

ShuntingYard::ShuntingYard(std::string data) : Parser(data) {
    // stub
}

void ShuntingYard::Error(std::string data) {
    throw "Parsing Error: " + data;
}

Expression* ShuntingYard::tokenToExp(Token tok) {
    if (TwoSidedOps.find(tok.getToken()) != TwoSidedOps.end()) {
        Expression *right = output.back();
        output.pop_back();
        Expression *left = output.back();
        output.pop_back();

        switch (tok.getToken()) {
            case tk_op_Divide:
                return new Division(left, right);
            case tk_op_Minus:
                return new Subtraction(left, right);
            case tk_op_Multiply:
                return new Multiplication(left, right);
            case tk_op_Plus:
                return new Addition(left, right);
            case tk_op_Power:
                return new Exponentiation(left, right);
            case tk_op_Rt:
                return new NthRoot(left, right);
            case tk_op_Log:
                return new Logarithm(left, right);
            default: ;
        }
    }

    if (OneSidedOps.find(tok.getToken()) != OneSidedOps.end()) {
        Expression *right = output.back();
        output.pop_back();

        switch (tok.getToken()) {
            case tk_op_Ln:
                return new LogNatural(right);
            default: ;
        }
    }

    if (tok.getToken() == tk_Method) {
        Expression* expr;
        if (!Method::find(tok.getData(), expr))
            throw "It's impossible.";
        Method* method = dynamic_cast<Method*>(expr);

        ExpressionList params;
        int paramCount = method->getParamCount();
        for (int i = 0; i < paramCount; i++) {
            Expression* param = output.back();
            output.pop_back();
            params.push_front(param);
        }

        return method->call(params);
    }

    Error("Unknown token (" + tok.getData() + ")");
    return NULL; // Error throws an exception, need this to suppress warning.
}

void ShuntingYard::pushOp(Token token) {
    while ((opers.size() > 0) && ((opers.top().getPrecedence() > token.getPrecedence()) ||
                                  ((token.getPrecedence() == opers.top().getPrecedence()) && (token.getAssoc() == assocLeft)))) {
        output.push_back(tokenToExp(opers.top()));
        opers.pop();
    }

    opers.push(token);
}

Expression* ShuntingYard::process() {

    EParserToken lastToken = tk_NULL;
    while (nextTokenNoJunk() != tk_NULL) {
        Token token = getToken();

        switch (token.getToken()) {
            case tk_Identifier:
                if ((lastToken == tk_typ_Integer) || (lastToken == tk_typ_Float))
                    pushOp(Token(tk_op_Multiply, 0, "*"));

                Expression* temp;
                if (Method::find(token.getData(), temp))
                    pushOp(Token(tk_Method, token.getPos(), token.getData()));
                else if (Variable::find(token.getData(), temp))
                    output.push_back(temp);
                else
                    output.push_back(new Variable(token.getData(), NULL));
                break;
            case tk_typ_Integer:
                if ((lastToken == tk_typ_Integer) && (*(output.back()) == *negOne)) {
                    output.pop_back();
                    output.push_back(new Integer(strToT<int>("-" + token.getData())));
                } else
                    output.push_back(new Integer(strToT<int>(token.getData())));
                break;
            case tk_typ_Float:
                if ((lastToken == tk_typ_Integer) && (*(output.back()) == *negOne)) {
                    output.pop_back();
                    output.push_back(new Float(strToT<float>("-" + token.getData())));
                } else
                    output.push_back(new Float(strToT<float>(token.getData())));
                break;
            case tk_op_Minus:
                switch (lastToken) {
                    case tk_NULL:
                    case tk_sym_ParenthesisOpen:
					case tk_op_Divide: case tk_op_Ln: case tk_op_Log: case tk_op_Minus:
					case tk_op_Multiply: case tk_op_Plus: case tk_op_Power: case tk_op_Rt:
                        token = Token(tk_typ_Integer, 0, "-1");
                        output.push_back(negOne);
                        break;
                    default:
                        pushOp(token);
                        break;
                }
                break;
			case tk_op_Divide: case tk_op_Ln: case tk_op_Log:
			case tk_op_Multiply: case tk_op_Plus: case tk_op_Power: case tk_op_Rt:
                pushOp(token);
                break;
            case tk_sym_ParenthesisOpen: case tk_sym_BracketOpen:
                if ((lastToken == tk_sym_ParenthesisClose) || (lastToken == tk_typ_Integer) || (lastToken == tk_typ_Float))
                    pushOp(Token(tk_op_Multiply, 0, "*"));
                opers.push(token);
                break;
            case tk_sym_ParenthesisClose:
                while (opers.top().getToken() != tk_sym_ParenthesisOpen) {
                    output.push_back(tokenToExp(opers.top()));
                    opers.pop();
                }
                opers.pop();
                break;
            case tk_sym_BracketClose:
                while (opers.top().getToken() != tk_sym_BracketOpen) {
                    output.push_back(tokenToExp(opers.top()));
                    opers.pop();
                }
                opers.pop();
                break;
            default:
                throw "What??";
        }

        lastToken = token.getToken();
    }

    while (opers.size() > 0) {
        output.push_back(tokenToExp(opers.top()));
        opers.pop();
    }

    if (output.size() > 1) {
        Error("Something was not right...");
    }

    return output.front();
}
