#include "../include/env.h"

object Env::at_key(std::string key) 
{
    for (int i = 0; i < (int) keys.size(); i++) 
    {
        std::string key_comp = keys.at(i);
        if (key_comp == key) 
        {
            return values.at(i);
        }
    }
    return make_object(BOP_NUMBER, "0");
}

bool Env::has_key(std::string key) 
{
    for (int i = 0; i < (int) keys.size(); i++) 
    {
        std::string key_comp = keys.at(i);
        if (key_comp == key) 
        {
            return true;
        }
    }
    return false;
}

void Env::add_key(std::string key, object value) 
{
    keys.push_back(key);
    values.push_back(value);
}

void Env::set_key(std::string key, object value) 
{
    if (has_key(key)) 
    {
        for (int i = 0; i < (int) keys.size(); i++) 
        {
            std::string key_comp = keys.at(i);
            if (key_comp == key) 
            {
                values[i] = value; return;
            }
        }
    } 
    else 
    {
        add_key(key, value);
    }
}

void Env::print() 
{
    for (int i = 0; i < (int) keys.size(); i++) 
    {
        std::string key = keys.at(i);
        object value = values.at(i);
        std::cout << key << "  :  " << value.value << std::endl;
    }
}