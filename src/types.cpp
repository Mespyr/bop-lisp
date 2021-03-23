#include "../include/types.h"


std::string format_string(std::string str) {
    return str.substr(1, str.size()-2);
}

std::string repr(object obj) {
    if (obj.type == BOP_NUMBER) return obj.value;
    return format_string(obj.value);
} 

double to_number(std::string fl) {
    return atof(fl.c_str());
}

object make_object(Type type_, std::string value) {
    object obj;
    obj.type = type_;
    obj.value = value;
    return obj;
}