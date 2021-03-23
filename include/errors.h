#ifndef BOP_ERRORS_H
#define BOP_ERRORS_H

#include "token.h"

typedef enum {
    ARGUMENT_ERROR, KEYWORD_ERROR
} ErrorType;


struct ParsingError
{
    std::string msg;
    Token tok;
    bool empty_stream = false;
    std::string error();
};


struct ArgumentError {
    std::string msg;
    unsigned int line;
    std::string error();
};
struct KeywordError {
    std::string msg;
    unsigned int line;
    std::string error();
};


struct ErrorMap {
    ErrorType type;
    ArgumentError arg;
    KeywordError keyword;
    std::string error();
};


#endif