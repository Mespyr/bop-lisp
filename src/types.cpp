#include "../include/types.h"


std::string format_string(std::string str) 
{
    return str.substr(1, str.size()-2);
}

std::string repr(object obj) 
{
    if (obj.type == BOP_NUMBER) 
    {
        return obj.value;
    }
    else if (obj.type == BOP_LIST) 
    {
        return "<List>";
    }
    else 
    {
        return format_string(obj.value);
    }
} 

double to_number(std::string fl) 
{
    return atof(fl.c_str());
}

object make_object(Type type_, std::string value, std::vector<object> list) 
{
    object obj;
    obj.type = type_;

    if (type_ == BOP_LIST) 
    {
        obj.list = list;
    } 
    else 
    {
        obj.value = value;
    }
    return obj;
}