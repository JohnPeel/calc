#include "Parser.h"

const char* ParserTokenStr[] = {
        "NULL",
        "ERROR",
        "Identifier",
        "Method",
        "WhiteSpace",
        "NewLine",

        "Equal",
        "GreaterThan",
        "GreaterThanOrEqual",
        "LessThan",
        "LessThanOrEqual",
        "NotEqual",

        "Divide",
        "Ln",
        "Log"
                "Minus",
        "Multiply",
        "Plus",
        "Power",
        "Root",

        "BracketOpen",
        "BracketClose",
        "ParenthesisClose",
        "ParenthesisOpen",

        "Float",
        "Integer"
};

EOperatorAssociative OperatorAssociative[] = {
        assocNone, //tk_NULL,
        assocNone, //tk_ERROR,
        assocNone, //tk_Identifier,
        assocRight, //tk_Method,
        assocNone, //tk_WhiteSpace,
        assocNone, //tk_NewLine,

        assocLeft, //tk_cmp_Equal,
        assocLeft, //tk_cmp_GreaterThan,
        assocLeft, //tk_cmp_GreaterThanOrEqual,
        assocLeft, //tk_cmp_LessThan,
        assocLeft, //tk_cmp_LessThanOrEqual,
        assocLeft, //tk_cmp_NotEqual,

        assocLeft, //tk_op_Divide,
        assocRight, //tk_op_Ln
        assocRight, //tk_op_Log
        assocLeft, //tk_op_Minus,
        assocLeft, //tk_op_Multiply,
        assocLeft, //tk_op_Plus,
        assocRight, //tk_op_Power,
        assocRight, //tk_op_Rt,

        assocNone, //tk_sym_BracketOpen,
        assocNone, //tk_sym_BracketClose,
        assocNone, //tk_sym_ParenthesisClose,
        assocNone, //tk_sym_ParenthesisOpen,

        assocNone, //tk_typ_Float,
        assocNone  //tk_typ_Integer
};

unsigned char OperatorPrecedence[] = {
        0, //tk_NULL,
        0, //tk_ERROR,
        5, //tk_Identifier, // Characters
        5, //tk_Method,
        0, //tk_WhiteSpace,
        0, //tk_NewLine,

        0, //tk_cmp_Equal,
        0, //tk_cmp_GreaterThan,
        0, //tk_cmp_GreaterThanOrEqual,
        0, //tk_cmp_LessThan,
        0, //tk_cmp_LessThanOrEqual,
        0, //tk_cmp_NotEqual,

        3, //tk_op_Divide,
        4, //tk_op_Ln
        4, //tk_op_Log
        2, //tk_op_Minus,
        3, //tk_op_Multiply,
        2, //tk_op_Plus,
        4, //tk_op_Power,
        4, //tk_op_Rt,

        1, //tk_sym_BracketOpen,
        0, //tk_sym_BracketClose,
        0, //tk_sym_ParenthesisClose,
        1, //tk_sym_ParenthesisOpen,

        0, //tk_typ_Float,
        0  //tk_typ_Integer
};

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

void Parser::Expected(EParserToken token) {
    if (peek() != token)
        Error("Expected " + std::string(ParserTokenStr[token]));
}

void Parser::Expected(std::vector<EParserToken> tokens) {
    for (EParserToken token : tokens)
        Expected(token);
}

EParserToken Parser::nextToken() {
    token = tk_NULL;
    tokenPos = pos;

    if (pos >= data.length())
        return tk_NULL;

    switch (data[pos]) {
        case '\0':
            token = tk_NULL;
            break;
        case '\n': case '\r':
            if ((data[pos] == '\r') && (data[pos + 1] == '\n'))
                pos++;
            pos++;
            token = tk_NewLine;
            break;
        case '.': case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
            token = tk_typ_Integer;
            while (((data[pos] >= '0') && (data[pos] <= '9')) || (data[pos] == '.')) {
                if (data[pos] == '.')
                    token = tk_typ_Float;
                pos++;
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
        default:
            if ((data[pos] >= '\x01') && (data[pos] <= '\x32')) {
                pos++;
                while ((data[pos] >= char(1)) && (data[pos] <= char(32)) && ((data[pos] != '\n') || (data[pos] != '\r')))
                    pos++;
                token = tk_WhiteSpace;
            } else if (((data[pos] >= 'a') && (data[pos] <= 'z')) || ((data[pos] >= 'A') && (data[pos] <= 'Z'))) {
				while (((data[pos] >= 'a') && (data[pos] <= 'z')) || ((data[pos] >= 'A') && (data[pos] <= 'Z')))
					pos++;
				
				token = tk_Identifier;

				std::string ident = data.substr(tokenPos, pos - tokenPos);
				std::transform(ident.begin(), ident.end(), ident.begin(), ::tolower);

				if (ident == "rt")
					token = tk_op_Rt;
				if (ident == "ln")
					token = tk_op_Ln;
				if (ident == "log")
					token = tk_op_Log;
			} else {
				pos++;
				token = tk_ERROR;
			}
    }

    return token;
}

EParserToken Parser::nextTokenNoJunk() {
    EParserToken tok = nextToken();
    while ((token == tk_NewLine) || (token == tk_WhiteSpace))
        tok = nextToken();
    return tok;
}

EParserToken Parser::peek() {
    unsigned int curPos = pos;
    EParserToken ret = nextToken();
    pos = curPos;
    return ret;
}

EParserToken Parser::peekNoJunk() {
    unsigned int curPos = pos;
    EParserToken ret = nextTokenNoJunk();
    pos = curPos;
    return ret;
}

Token Parser::getToken() {
    return Token(token, tokenPos, data.substr(tokenPos, pos - tokenPos));
}