#ifndef BOP_TYPES_H
#define BOP_TYPES_H

#include <string>
#include <vector>

// Parsing Types ##########################################

// Different types of nodes
enum NodeType 
{
    LIST, ATOM
};

// Atom types which are determined when parsing
enum AtomType 
{
    NUMBER,
    STRING,
    SYMBOL 
};

// ########################################################


enum Type 
{
    BOP_STRING, 
    BOP_NUMBER, 
    BOP_LIST
};


struct object
{
    Type type;
    std::string value;
    std::vector<object> list;
};


// Fucntions
std::string format_string(std::string str);
double to_number(std::string fl);
std::string repr(object obj);

object make_object(Type type_, std::string value = "", std::vector<object> list = {});


#endif