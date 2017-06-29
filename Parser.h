#ifndef CALC_PARSER_H
#define CALC_PARSER_H

#include <algorithm>
#include <string>
#include <vector>

enum EParserToken {
    tk_NULL = 0,
    tk_ERROR,
    tk_Identifier,
    tk_Method,
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

extern const char* ParserTokenStr[];

enum EOperatorAssociative {
    assocNone = 0,
    assocLeft,
    assocRight
};

extern EOperatorAssociative OperatorAssociative[];
extern unsigned char OperatorPrecedence[];

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
