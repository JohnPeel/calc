#include "Parser.h"

Token::Token(EParserToken token, unsigned int pos, std::string data) {
    this->token = token;
    this->pos = pos;
    this->data = data;
}

EParserToken Token::getToken() {
    return token;
}

unsigned int Token::getPos() {
    return pos;
}

std::string Token::getData() {
    return data;
}

unsigned char Token::getPrecedence() {
    return OperatorPrecedence[token];
}

EOperatorAssociative Token::getAssoc() {
    return OperatorAssociative[token];
}

Parser::Parser(std::string data) {
    this->data = data;
    reset();
}

void Parser::reset() {
    pos = 0;
    token = tk_NULL;
}

EParserToken Parser::nextToken() {
    token = tk_NULL;
    tokenPos = pos;

    if ((pos < 0) or (pos >= data.length()))
        return tk_NULL;

    switch (data[pos]) {
        case '\0':
            token = tk_NULL;
            break;
        case '\n': case '\r':
            if ((data[pos] == '\r') and (data[pos + 1] == '\n'))
                pos++;
            pos++;
            token = tk_NewLine;
            break;
        case char(1) ... '\t': case '\v': case '\f': case char(14) ... char(32):
            pos++;
            while ((data[pos] >= char(1)) and (data[pos] <= char(32)) and ((data[pos] != '\n') or (data[pos] != '\r')))
                pos++;
            token = tk_WhiteSpace;
            break;
        case '0' ... '9':
            token = tk_typ_Integer;
            while (((data[pos] >= '0') and (data[pos] <= '9')) or (data[pos] == '.')) {
                if (data[pos] == '.')
                    token = tk_typ_Float;
                pos++;
            }
            break;
        case 'a' ... 'z': case 'A' ... 'Z':
            while (((data[pos] >= 'a') and (data[pos] <= 'z')) or ((data[pos] >= 'A') and (data[pos] <= 'Z')))
                pos++;
            token = tk_Identifier;

            { // We need this block because of the variable below.
                std::string ident = data.substr(tokenPos, pos - tokenPos);
                std::transform(ident.begin(), ident.end(), ident.begin(), ::tolower);

                if (ident == "rt")
                    token = tk_op_Rt;
            }

            break;
        case '=':
            pos++;
            token = tk_cmp_Equal;
            break;
        case '>':
            pos++;
            token = tk_cmp_GreaterThan;

            if (data[pos] == '=') {
                pos++;
                token = tk_cmp_GreaterThanOrEqual;
            }

            break;
        case '<':
            pos++;
            token = tk_cmp_LessThan;

            if (data[pos] == '=') {
                pos++;
                token = tk_cmp_LessThanOrEqual;
            }

            if (data[pos] == '>') {
                pos++;
                token = tk_cmp_NotEqual;
            }

            break;
        case '/':
            pos++;
            token = tk_op_Divide;
            break;
        case '-':
            pos++;
            token = tk_op_Minus;
            break;
        case '*':
            pos++;
            token = tk_op_Multiply;
            if (data[pos] == '*') {
                pos++;
                token = tk_op_Power;
            }
            break;
        case '+':
            pos++;
            token = tk_op_Plus;
            break;
        case '^':
            pos++;
            token = tk_op_Power;
            break;
        case '[':
            pos++;
            token = tk_sym_BracketOpen;
            break;
        case ']':
            pos++;
            token = tk_sym_BracketClose;
            break;
        case '(':
            pos++;
            token = tk_sym_ParenthesisOpen;
            break;
        case ')':
            pos++;
            token = tk_sym_ParenthesisClose;
            break;
        default: // Unknown?
            pos++;
            token = tk_ERROR;
            break;
    }

    return token;
}

EParserToken Parser::nextTokenNoJunk() {
    EParserToken tok = nextToken();
    while ((token == tk_NewLine) or (token == tk_WhiteSpace))
        tok = nextToken();
    return tok;
}

Token Parser::getToken() {
    return Token(token, tokenPos, data.substr(tokenPos, pos - tokenPos));
}