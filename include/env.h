#ifndef BOP_ENV_H
#define BOP_ENV_H

#include <vector>
#include <string>
#include <iostream>

#include "types.h"

class Env 
{
public:
    std::vector<std::string> keys;
    std::vector<object> values;

    object at_key(std::string key);
    bool has_key(std::string key);
    void add_key(std::string key, object value);
    void set_key(std::string key, object value);
    void print();
};

class Env_Stack 
{
public:
    std::vector<Env> stack{Env{}};

    Env back() 
    {
        return stack.back();
    }

    void push(Env e) 
    {
        stack.push_back(e);
    } 

    void pop() 
    {
        stack.pop_back();
    }

    void new_() 
    {
        Env n = back();
        stack.push_back(n);
    }
};

#endif