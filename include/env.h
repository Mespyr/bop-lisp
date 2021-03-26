#ifndef BOP_ENV_H
#define BOP_ENV_H

#include <string>
#include <iostream>
#include <map>

#include "types.h"

class Env 
{
public:
    std::map<std::string, object> dict;

    object at_key(std::string key);
    bool has_key(std::string key);
    void add_key(std::string key, object value);
    void set_key(std::string key, object value);
};

class Env_Stack 
{
public:
    std::vector<Env> stack{Env{}};

    Env back();
    void push(Env e);
    void pop();
    void new_();
};

#endif