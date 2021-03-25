#ifndef BOP_EVAL_H
#define BOP_EVAL_H

#include "parser.h"
#include "types.h"
#include "errors.h"
#include "env.h"

#include <vector>

class AST_Handler 
{
public:
    AST ast;
    unsigned int ptr;
    bool eof = false;
    Node next();
};

class Evaluator 
{
public:
    Env_Stack env_stack;
    ErrorMap error;
    bool error_found = false;
    object evaluate(Node node);
};


// Builtin Functions
std::string Function_format_string(std::vector<object> objs);
std::string Function_add_nums(std::vector<object> objs);
std::string Function_subtract_nums(std::vector<object> objs);
std::string Function_times_nums(std::vector<object> objs);
std::string Function_div_nums(std::vector<object> objs);

object Null();

#endif