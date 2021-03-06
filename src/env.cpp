#include "../include/env.h"

// Env
object Env::at_key(std::string key) 
{
    if (has_key(key))
    {
        return dict.at(key);
    }
    else 
    {
        return make_object(BOP_NUMBER, "0");
    }
}

bool Env::has_key(std::string key) 
{
    return dict.count(key);
}

void Env::add_key(std::string key, object value) 
{
    dict.insert(std::pair<std::string, object>(key, value));
}

void Env::set_key(std::string key, object value) 
{
    if (has_key(key)) 
    {
        dict.find(key)->second = value;
    } 
    else 
    {
        add_key(key, value);
    }
}


// Env Stack
Env Env_Stack::back() 
{
    return stack.back();
}

void Env_Stack::push(Env e) 
{
    stack.push_back(e);
} 

void Env_Stack::pop() 
{
    stack.pop_back();
}

void Env_Stack::new_() 
{
    Env n = back();
    stack.push_back(n);
}