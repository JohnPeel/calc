#ifndef CALC_PARSER_H
#define CALC_PARSER_H

#include <algorithm>
#include <string>
#include <vector>

enum EParserToken {
    tk_NULL = 0,
    tk_ERROR,
    tk_Identifier,
    tk_WhiteSpace,
    tk_NewLine,

    tk_cmp_Equal,
    tk_cmp_GreaterThan,
    tk_cmp_GreaterThanOrEqual,
    tk_cmp_LessThan,
    tk_cmp_LessThanOrEqual,
    tk_cmp_NotEqual,

    tk_op_Divide,
    tk_op_Ln,
    tk_op_Log,
    tk_op_Minus,
    tk_op_Multiply,
    tk_op_Plus,
    tk_op_Power,
    tk_op_Rt,

    tk_sym_BracketOpen,
    tk_sym_BracketClose,
    tk_sym_ParenthesisClose,
    tk_sym_ParenthesisOpen,

    tk_typ_Float,
    tk_typ_Integer
};

static const char* ParserTokenStr[] = {
        "NULL",
        "ERROR",
        "Identifier",
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

enum EOperatorAssociative {
    assocNone = 0,
    assocLeft,
    assocRight
};

static EOperatorAssociative OperatorAssociative[] = {
        assocNone, //tk_NULL,
        assocNone, //tk_ERROR,
        assocNone, //tk_Ientifier,
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

static unsigned char OperatorPrecedence[] = {
        0, //tk_NULL,
        0, //tk_ERROR,
        5, //tk_Identifier, // Functions and Variables
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

class Token {
private:
    EParserToken token;
    unsigned int pos;
    std::string data;
public:
    Token(EParserToken, unsigned int, std::string);
    EParserToken getToken();
    unsigned int getPos();
    std::string getData();
    unsigned char getPrecedence();
    EOperatorAssociative getAssoc();
};

class Parser {
protected:
    std::string data;
    unsigned int pos, tokenPos;
    EParserToken token;
public:
    Parser(std::string data);
    void reset();

    virtual void Error(std::string) = 0;
    void Expected(EParserToken);
    void Expected(std::vector<EParserToken>);

    EParserToken nextToken();
    EParserToken nextTokenNoJunk();
    EParserToken peek();
    EParserToken peekNoJunk();

    Token getToken();
};


#endif //CALC_PARSER_H
