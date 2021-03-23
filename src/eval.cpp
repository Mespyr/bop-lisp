#include "../include/eval.h"

Node AST_Handler::next() {
    ptr++;
    if (ptr >= (int) ast.nodes.size()) eof = true;
    return ast.nodes.at(ptr-1);
}

object Null() {
    return make_object(BOP_NUMBER, "0");
}


// formatting strings
std::string Function_format_string(std::vector<object> objs) {
    std::string res;
    for (int i = 0; i < (int)objs.size(); i++) {
        res += repr(objs.at(i));
    }
    return "'"+res+"'";
}


// Math stuff
std::string Function_add_nums(std::vector<object> objs) {
    float res = 0;
    for (int i = 0; i < (int)objs.size(); i++) {
        res += atof(repr(objs.at(i)).c_str());
    }
    return std::to_string(res);
}
std::string Function_subtract_nums(std::vector<object> objs) {
    float res = atof(repr(
                        objs.at(0)).c_str());
    for (int i = 1; i < (int)objs.size(); i++) {
        res -= atof(repr(objs.at(i)).c_str());
    }
    return std::to_string(res);
}
std::string Function_times_nums(std::vector<object> objs) {
    float res = atof(repr(
                        objs.at(0)).c_str());
    for (int i = 1; i < (int)objs.size(); i++) {
        res *= atof(repr(objs.at(i)).c_str());
    }
    return std::to_string(res);
}
std::string Function_div_nums(std::vector<object> objs) {
    float res = atof(repr(
                        objs.at(0)).c_str());
    for (int i = 1; i < (int)objs.size(); i++) {
        res /= atof(repr(objs.at(i)).c_str());
    }
    return std::to_string(res);
}



object Evaluator::evaluate(Node node) {
    if (node.type == ATOM) {
        Atom atom = node.atom;
        if (atom.type == NUMBER)
            return make_object(BOP_NUMBER, atom.value);
        else if (atom.type == STRING)
            return make_object(BOP_STRING, atom.value); 
        else 
            return make_object(BOP_STRING, "SYMBOL");

    } else {
        if (node.nodes.front().atom.value == "println") {
            object obj = evaluate(node.nodes.back());
            if (error_found) return Null();
            std::cout << repr(obj) << std::endl;
            return Null();
        } 
        else if (node.nodes.front().atom.value == "print") {
            object obj = evaluate(node.nodes.back());
            if (error_found) return Null();
            std::cout << repr(obj);
            return Null();
        } 
        else if (node.nodes.front().atom.value == "format") {
            if ( (int) node.nodes.size() == 1) {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"No arguments passed for 'format'.", node.nodes.front().atom.line};
            }
            std::vector<object> formatters;
            for (int i = 1; i < (int) node.nodes.size(); i++) {
                Node n = node.nodes.at(i);
                formatters.push_back(evaluate(n));
                if (error_found) return Null();
            }
            return make_object(BOP_STRING, Function_format_string(formatters));
        }
        
        // Add
        else if (node.nodes.front().atom.value == "+") {
            if ( (int) node.nodes.size() == 1) {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"No arguments passed for '+'.", node.nodes.front().atom.line};
            }
            std::vector<object> formatters;
            for (int i = 1; i < (int) node.nodes.size(); i++) {
                Node n = node.nodes.at(i);
                formatters.push_back(evaluate(n));
                if (error_found) return Null();
            }
            return make_object(BOP_NUMBER, Function_add_nums(formatters));
        }
        
        // Subtract
        else if (node.nodes.front().atom.value == "-") {
            if ( (int) node.nodes.size() == 1) {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"No arguments passed for '-'.", node.nodes.front().atom.line};
            }
            std::vector<object> formatters;
            for (int i = 1; i < (int) node.nodes.size(); i++) {
                Node n = node.nodes.at(i);
                formatters.push_back(evaluate(n));
                if (error_found) return Null();
            }
            return make_object(BOP_NUMBER, Function_subtract_nums(formatters));
        }
        
        // Multiply
        else if (node.nodes.front().atom.value == "*") {
            if ( (int) node.nodes.size() == 1) {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"No arguments passed for '*'.", node.nodes.front().atom.line};
            }
            std::vector<object> formatters;
            for (int i = 1; i < (int) node.nodes.size(); i++) {
                Node n = node.nodes.at(i);
                formatters.push_back(evaluate(n));
                if (error_found) return Null();
            }
            return make_object(BOP_NUMBER, Function_times_nums(formatters));
        }
        
        // Divide
        else if (node.nodes.front().atom.value == "/") {
            if ( (int) node.nodes.size() == 1) {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"No arguments passed for '/'.", node.nodes.front().atom.line};
            }
            std::vector<object> formatters;
            for (int i = 1; i < (int) node.nodes.size(); i++) {
                Node n = node.nodes.at(i);
                formatters.push_back(evaluate(n));
                if (error_found) return Null();
            }
            return make_object(BOP_NUMBER, Function_div_nums(formatters));
        }

        else {
                error_found = true;
                error.type = KEYWORD_ERROR;
                error.keyword = KeywordError{"Unrecogonized keyword '" + 
                                                node.nodes.front().atom.value + "'.", 
                    node.nodes.front().atom.line};
        }
    }
    return Null();
}