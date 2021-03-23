#ifndef BOP_TOKEN_H
#define BOP_TOKEN_H

#include <string>
#include <vector>
#include <iostream>

// Token struct
struct _Token
{
    unsigned int line;
    std::string value;
};
typedef struct _Token Token;


// List of tokens
struct _TokenList
{
    std::vector<Token> token_stream = {};
    void out()
    {
        for (int i = 0; i < token_stream.size(); i++)
        {
            Token tok = token_stream.at(i);
            std::cout << tok.value << ", " << tok.line << std::endl;
        }
    }
};
typedef struct _TokenList TokenList;


#endif