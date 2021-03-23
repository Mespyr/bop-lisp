#ifndef BOP_PARSER_H
#define BOP_PARSER_H

#include "token.h"
#include "types.h"
#include "errors.h"


// Atom
struct Atom {
    std::string value;
    unsigned int line;
    AtomType type;
    Token token;
};


// Node, which can either be an Atom or a List
struct Node {
    NodeType type;
    std::vector<Node> nodes;
    Atom atom;
};


// AST which holds nodes, and errors
struct AST {
    std::vector<Node> nodes;
    bool error_found = false;
    ParsingError error;
};



// Reader, which feeds the next token to parse into AST
struct Reader {
    TokenList tokens;
    int ptr;
    bool eof = false;

    Token next() {
        ptr++;
        if (ptr >= (int) tokens.token_stream.size()) {
            eof = true;
        }
        return tokens.token_stream.at(ptr-1);
    }
    Token peek() {
        return tokens.token_stream.at(ptr);
    }

    ParsingError error;
    bool error_found = false;
};


// Functions
TokenList tokenize(std::string code);
AST parse(TokenList tokens);

Atom create_atom(Token tok);

// Check type of token
bool is_number(std::string value);
bool is_num(char c);

#endif