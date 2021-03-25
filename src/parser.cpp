#include "../include/parser.h"


TokenList tokenize(std::string code) {
    TokenList stream;
    std::string word;
    bool string_has_started = false;
    bool in_comment = false;

    unsigned int line = 1;

    for (int i = 0; i < code.length(); i++)
    {
        char buf = code.at(i);
        if (buf == '\n') line++;

        if (buf == ';' and !string_has_started) {
            in_comment = !in_comment;
        } else if(in_comment);

        else if (buf == '\'') {
            word.push_back(buf);
            if(!string_has_started) {
                string_has_started = true;
            } else {
                Token tok{line, word};
                stream.token_stream.push_back(tok);
                string_has_started = false;
                word = "";
            }

        } else if (string_has_started) word.push_back(buf);
        
        else if (isspace(buf)) {
            if (word != "") {
                Token tok{line, word};
                stream.token_stream.push_back(tok);
            }
            word = "";
        } 
        else if (buf == '(') {
            if (word != "") {
                Token tok{line, word};
                stream.token_stream.push_back(tok);
            }
            Token tok{line, "("};
            stream.token_stream.push_back(tok);
            word = "";
        }
        else if (buf == ')') {
            if (word != "") {
                Token tok{line, word};
                stream.token_stream.push_back(tok);
            }
            Token tok{line, ")"};
            stream.token_stream.push_back(tok);
            word = "";
        }
        else word.push_back(buf);
    }

    return stream;
}


// If char is number 
bool is_num(char c) {
    int t = c - 48; // Convert char from ascii to integer
    return (t > -1) * (t < 10); // if int is greater than -1 and t is less than 10, return true
}

// Check token
bool is_number(std::string value) {
    bool ret = false;
    
    bool dec_point_found = false;
    if (value.length() >= 2 and value.at(1) == '-') value = value.substr(1); // If value is negative

    for (int i = 0; i < value.length(); i++) {
        char t = value.at(i);
        ret = is_num(t);
        if (t == '.') {
            if (!dec_point_found) {
                dec_point_found = true;  // if no other decimal points then true
                ret = true; 
            } else ret = false;  // if there is more than one point, it cant be a float
        } // If char is decimal point
        if (!ret) break;

    } return ret;
}



AtomType Typeof(std::string value) {
    if (value.back() == '\'' and value.front() == '\'') return STRING;
    else if (is_number(value)) return NUMBER;
    return SYMBOL;
}


Atom create_atom(Token tok)  {
    Atom atom{};
    atom.type = Typeof(tok.value);
    atom.value = tok.value;
    atom.line = tok.line;
    atom.token = tok;
    return atom;
}


Node read_next(Reader& reader) {
    Token token = reader.next();


    // Detect EOF and raise error
    if (reader.eof == true) {
        reader.error = ParsingError{"Unexpected EOF while parsing.", token};
        reader.error_found = true;
        return Node{ATOM};
    }

    if (token.value == "(") {
        Node node{LIST};
        token = reader.peek();
        while (token.value != ")")
        {
            node.nodes.push_back(read_next(reader));
            if (reader.eof == true) {
                reader.error = ParsingError{"Unexpected EOF while parsing.", token};
                reader.error_found = true;
                return node;
            }
            token = reader.peek();
        }
        reader.next();
        return node;
    } else if (token.value == ")") {
        reader.error = ParsingError{"Unexpected ')' token found while parsing.", token};
        reader.error_found = true;
        return Node{ATOM};
    } 

    Node node{ATOM};
    node.atom = create_atom(token);
    return node;
}


AST parse(TokenList tokens) {
    AST ast;
    Reader reader{tokens};

    if ((int) tokens.token_stream.size() == 0) {
        ast.error = ParsingError{"Unexpected EOF while parsing."};
        ast.error.empty_stream = true;
        ast.error_found = true;
        return ast;
    }

    while(!reader.eof) {
        Node node = read_next(reader);
        if (reader.error_found) {
            ast.error_found = true;
            ast.error = reader.error;
            break;
        } if (node.type == ATOM) {
            ast.error_found = true;
            ast.error = ParsingError{"Unexpected '" + node.atom.token.value + "' token found while parsing.", node.atom.token};
            break;
        }
        ast.nodes.push_back(node);
    }
    return ast;
}
