#ifndef BOP_ERRORS_H
#define BOP_ERRORS_H

#include "token.h"

enum ErrorType 
{
    ARGUMENT_ERROR, 
    KEYWORD_ERROR, 
    DIVISION_BY_ZERO_ERROR, 
    TYPE_ERROR,
    INDEX_ERROR
};


struct ParsingError 
{
    std::string msg;
    Token tok;
    bool empty_stream = false;
    std::string error();
};
struct ArgumentError 
{
    std::string msg;
    unsigned int line;
    std::string error();
};
struct KeywordError 
{
    std::string msg;
    unsigned int line;
    std::string error();
};
struct DivisionByZeroError 
{
    unsigned int line;
    std::string error();
};
struct TypeError 
{
    std::string msg;
    unsigned int line;
    std::string error();
};
struct IndexError 
{
    std::string msg;
    unsigned int line;
    std::string error();
};

struct ErrorMap 
{
    ErrorType type;
    ArgumentError arg;
    KeywordError keyword;
    TypeError type_;
    DivisionByZeroError division;
    IndexError index;
    std::string error();
};

#endif