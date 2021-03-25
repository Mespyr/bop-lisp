#include "../include/errors.h"

// Errors
std::string ParsingError::error() 
{
    if (!empty_stream) 
    {
        std::string error_string;
        error_string += "Parsing Error on line " + std::to_string(tok.line) + ".\n";
        error_string += msg;
        return error_string;
    } 
    else 
    {
        std::string error_string;
        error_string += "Parsing Error on line 1.\n";
        error_string += msg;
        return error_string;
    }
}

std::string ArgumentError::error() 
{
    std::string error_string;
    error_string += "Argument Error on line " + std::to_string(line) + ".\n";
    error_string += msg;
    return error_string;
}
std::string KeywordError::error() 
{
    std::string error_string;
    error_string += "Keyword Error on line " + std::to_string(line) + ".\n";
    error_string += msg;
    return error_string;
}
std::string DivisionByZeroError::error() 
{
    std::string error_string;
    error_string += "DivisionByZero Error on line " + std::to_string(line) + ".\n";
    error_string += "Can't divide number by 0.";
    return error_string;
}
std::string TypeError::error() 
{
    std::string error_string;
    error_string += "Type Error on line " + std::to_string(line) + ".\n";
    error_string += msg;
    return error_string;
}
std::string IndexError::error() 
{
    std::string error_string;
    error_string += "Index Error on line " + std::to_string(line) + ".\n";
    error_string += msg;
    return error_string;
}

// Map for default error
std::string ErrorMap::error() 
{
    if (type == ARGUMENT_ERROR) 
    {
        return arg.error();
    }
    else if (type == KEYWORD_ERROR) 
    {
        return keyword.error();
    }
    else if (type == DIVISION_BY_ZERO_ERROR) 
    {
        return division.error();
    }
    else if (type == TYPE_ERROR) 
    {
        return type_.error();
    }
    else if (type == INDEX_ERROR) 
    {
        return index.error();
    }
    else 
    {
        return "Error Found with Bop: Can't get Error Type.";
    }
}