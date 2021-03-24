#ifndef BOP_TYPES_H
#define BOP_TYPES_H

#include <string>
#include <vector>

// Parsing Types ##########################################

// Different types of nodes
enum _NodeType {
    LIST, ATOM
};
typedef enum _NodeType NodeType;

// Atom types which are determined when parsing
enum _AtomType {
    NUMBER,
    STRING,
    SYMBOL 
};
typedef enum _AtomType AtomType;


// ########################################################


enum _Type {
    BOP_STRING, BOP_NUMBER, BOP_LIST
};
typedef enum _Type Type;


struct object
{
    Type type;
    std::string value;
    std::vector<object> list;
};

// remove quotation marks off string.
std::string format_string(std::string str);
// Convert to number
double to_number(std::string fl);

// Get representable value of object
std::string repr(object obj);

// std::vector<object> empty_list;

// Make object 
object make_object(Type type_, std::string value = "", std::vector<object> list = {});


#endif