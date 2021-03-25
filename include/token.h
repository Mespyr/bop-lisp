#ifndef BOP_TOKEN_H
#define BOP_TOKEN_H

#include <string>
#include <vector>
#include <iostream>

// Token struct
class Token 
{
public:
    unsigned int line;
    std::string value;
};


// List of tokens
struct TokenList
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


#endif