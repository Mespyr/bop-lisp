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
        else {
            if (env_stack.back().has_key(atom.value)) return env_stack.back().at_key(atom.value); 
            else {
                error_found = true;
                error.type = KEYWORD_ERROR;
                error.keyword = KeywordError{"Undefined variable '" + atom.value + "'.", 
                    atom.line};
                return Null();
            }
        }

    } else {
        if (node.nodes.front().atom.value == "println") {
            if ((int) node.nodes.size() > 2) {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"Too many arguments passed for 'println'.", node.nodes.front().atom.line};
            } else if ((int) node.nodes.size() == 1) {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"No arguments passed for 'println'.", node.nodes.front().atom.line};
            }
            object obj = evaluate(node.nodes.back());
            if (error_found) return Null();
            std::cout << repr(obj) << std::endl;
            return Null();
        } 
        else if (node.nodes.front().atom.value == "print") {
            if ((int) node.nodes.size() > 2) {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"Too many arguments passed for 'print'.", node.nodes.front().atom.line};
            } else if ((int) node.nodes.size() == 1) {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"No arguments passed for 'print'.", node.nodes.front().atom.line};
            }
            object obj = evaluate(node.nodes.back());
            if (error_found) return Null();
            std::cout << repr(obj);
            return Null();
        } 
        else if (node.nodes.front().atom.value == "let") {
            if ((int) node.nodes.size() > 3) {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"Too many arguments passed for 'let'.", node.nodes.front().atom.line};
            } else if ((int) node.nodes.size() < 3) {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"Insufficient number of arguments passed for 'let'.", node.nodes.front().atom.line};
            }
            object obj = evaluate(node.nodes.back());
            if (node.nodes.at(1).atom.type != SYMBOL) {
                error_found = true;
                error.type = KEYWORD_ERROR;
                error.keyword = KeywordError{"Can't set variable name to non-symbol type.", 
                    node.nodes.front().atom.line};
            }
            if (error_found) return Null();
            Env env = env_stack.back();
            env.set_key(node.nodes.at(1).atom.value, obj);
            env_stack.pop();
            env_stack.push(env);
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
        
        // Math
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
                object o = evaluate(n);
                if (o.type != BOP_NUMBER) {
                    error_found = true;
                    error.type = TYPE_ERROR;
                    error.type_ = TypeError{"Can't add with non-numeric type.", node.nodes.front().atom.line};
                }
                formatters.push_back(o);
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
                object o = evaluate(n);
                if (o.type != BOP_NUMBER) {
                    error_found = true;
                    error.type = TYPE_ERROR;
                    error.type_ = TypeError{"Can't subtract with non-numeric type.", node.nodes.front().atom.line};
                }
                formatters.push_back(o);
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
                object o = evaluate(n);
                if (o.type != BOP_NUMBER) {
                    error_found = true;
                    error.type = TYPE_ERROR;
                    error.type_ = TypeError{"Can't multiply with non-numeric type.", node.nodes.front().atom.line};
                }
                formatters.push_back(o);
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
                object o = evaluate(n);
                if (o.type != BOP_NUMBER) {
                    error_found = true;
                    error.type = TYPE_ERROR;
                    error.type_ = TypeError{"Can't divide with non-numeric type.", node.nodes.front().atom.line};
                } else if (o.value == "0") {
                    error_found = true;
                    error.type = DIVISION_BY_ZERO_ERROR;
                    error.division = DivisionByZeroError{node.nodes.front().atom.line};
                }
                formatters.push_back(o);
                if (error_found) return Null();
            }
            return make_object(BOP_NUMBER, Function_div_nums(formatters));
        }



        // Indexing
        else if (node.nodes.front().atom.value == "list") {
            if ((int)node.nodes.size() == 1) return make_object(BOP_LIST, "");
            else {
                std::vector<object> formatters;
                for (int i = 1; i < (int) node.nodes.size(); i++) {
                    Node n = node.nodes.at(i);
                    object o = evaluate(n);
                    formatters.push_back(o);
                    if (error_found) return Null();
                }
                return make_object(BOP_LIST, "", formatters);
            }
        }
        else if (node.nodes.front().atom.value == "first") {
            if ( (int) node.nodes.size() == 1) {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"No arguments passed for 'first'.", node.nodes.front().atom.line};
            }
            else if ((int) node.nodes.size() > 2) {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"Too many arguments passed for 'first'.", node.nodes.front().atom.line};
            }
            object obj = evaluate(node.nodes.at(1));
            if (error_found) return Null();
            if (obj.type != BOP_LIST and obj.type != BOP_STRING) {
                error_found = true;
                error.type = TYPE_ERROR;
                error.type_ = TypeError{"Can't get first value on type that isn't string or list.", node.nodes.front().atom.line};
                return Null();
            }
            if (obj.type == BOP_LIST){
                if ((int)obj.list.size() == 0) {
                    error_found = true;
                    error.type = INDEX_ERROR;
                    error.index = IndexError{"Can't get first value from empty list.", node.nodes.front().atom.line};
                    return Null();
                }
                return obj.list.front();
            }
            else {
                if ((int)obj.value.length() == 0) {
                    error_found = true;
                    error.type = INDEX_ERROR;
                    error.index = IndexError{"Can't get first value from empty string.", node.nodes.front().atom.line};
                    return Null();
                }
                return make_object(BOP_STRING, "'"+repr(obj).substr(0, 1)+"'"); 
            }
        }
        else if (node.nodes.front().atom.value == "last") {
            if ( (int) node.nodes.size() == 1) {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"No arguments passed for last'.", node.nodes.front().atom.line};
            }
            else if ((int) node.nodes.size() > 2) {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"Too many arguments passed for 'last'.", node.nodes.front().atom.line};
            }
            object obj = evaluate(node.nodes.at(1));
            if (error_found)
                return Null();
            if (obj.type != BOP_LIST and obj.type != BOP_STRING) {
                error_found = true;
                error.type = TYPE_ERROR;
                error.type_ = TypeError{"Can't get last value on type that isn't string or list.", node.nodes.front().atom.line};
                return Null();
            }
            if (obj.type == BOP_LIST) {
                if ((int)obj.list.size() == 0) {
                    error_found = true;
                    error.type = INDEX_ERROR;
                    error.index = IndexError{"Can't get last value from empty list.", node.nodes.front().atom.line};
                    return Null();
                }
                return obj.list.back();
            } else {
                if ((int)obj.value.length() == 0) {
                    error_found = true;
                    error.type = INDEX_ERROR;
                    error.index = IndexError{"Can't get last value from empty string.", node.nodes.front().atom.line};
                    return Null();
                }
                std::string c;
                c.push_back(repr(obj).back());
                return make_object(BOP_STRING, "'" + c + "'"); 
            }
        }
        else if (node.nodes.front().atom.value == "rest") {
            if ( (int) node.nodes.size() == 1) {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"No arguments passed for 'rest'.", node.nodes.front().atom.line};
            }
            else if ((int) node.nodes.size() > 2) {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"Too many arguments passed for 'rest'.", node.nodes.front().atom.line};
            }
            object obj = evaluate(node.nodes.at(1));
            if (error_found) return Null();
            if (obj.type != BOP_LIST and obj.type != BOP_STRING) {
                error_found = true;
                error.type = TYPE_ERROR;
                error.type_ = TypeError{"Can't get values on type that isn't string or list.", node.nodes.front().atom.line};
                return Null();
            }
            if (obj.type == BOP_LIST) {
                if ((int)obj.list.size() == 0) {
                    error_found = true;
                    error.type = INDEX_ERROR;
                    error.index = IndexError{"Can't get the rest of a list that is empty.", node.nodes.front().atom.line};
                    return Null();
                }
                std::vector<object> sub_list;
                for (int i = 1; i < obj.list.size(); i++) {
                    sub_list.push_back(obj.list.at(i));
                }
                return make_object(BOP_LIST, "", sub_list);
            }
            else {
                if ((int)obj.value.length() == 0) {
                    error_found = true;
                    error.type = INDEX_ERROR;
                    error.index = IndexError{"Can't get the rest of a  string that is empty.", node.nodes.front().atom.line};
                    return Null();
                }
                return make_object(BOP_STRING, "'"+repr(obj).substr(1)+"'"); 
            }
        }
        else if (node.nodes.front().atom.value == "getf") {
            if ( (int) node.nodes.size() < 3) {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"Insufficent number of arguments passed for 'getf'.", node.nodes.front().atom.line};
            }
            else if ((int) node.nodes.size() > 3) {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"Too many arguments passed for 'getf'.", node.nodes.front().atom.line};
            }

            object obj = evaluate(node.nodes.at(1));
            if (error_found)return Null();
            object idx = evaluate(node.nodes.at(2));
            if (error_found)return Null();

            if (obj.type != BOP_LIST and obj.type != BOP_STRING) {
                error_found = true;
                error.type = TYPE_ERROR;
                error.type_ = TypeError{"Can't get value from type that isn't string or list.", node.nodes.front().atom.line};
                return Null();
            }
            if (idx.type != BOP_NUMBER) {
                error_found = true;
                error.type = TYPE_ERROR;
                error.type_ = TypeError{"Can't get use value that isn't number as index.", node.nodes.front().atom.line};
                return Null();
            } 
            
            if (obj.type == BOP_LIST) {
                if (atoi(idx.value.c_str()) >= (int)obj.list.size()) {
                    error_found = true;
                    error.type = INDEX_ERROR;
                    error.index = IndexError{"Index " + idx.value + " out of range.", node.nodes.front().atom.line};
                    return Null();
                } 
                return obj.list.at(atoi(idx.value.c_str()));
            } else if (obj.type == BOP_STRING) {
                if (atoi(idx.value.c_str()) >= (int)obj.value.length()) {
                    error_found = true;
                    error.type = INDEX_ERROR;
                    error.index = IndexError{"Index " + idx.value + " out of range.", node.nodes.front().atom.line};
                    return Null();
                }
                int idx_ = atoi(idx.value.c_str());
                std::string str = "'"; 
                str.push_back(obj.value.at(idx_));
                str.push_back('\'');

                return make_object(BOP_STRING, str);
            }

        }

        else if (node.nodes.front().atom.value == "append") {
            if ( (int) node.nodes.size() < 3) {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"Insufficent number of arguments passed for 'append'.", node.nodes.front().atom.line};
            }
            else if ((int) node.nodes.size() > 3) {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"Too many arguments passed for 'append'.", node.nodes.front().atom.line};
            }

            object obj = evaluate(node.nodes.at(2));
            if (error_found) return Null();
            object appender = evaluate(node.nodes.at(1));
            if (error_found) return Null();

            if (obj.type != BOP_LIST) {
                error_found = true;
                error.type = TYPE_ERROR;
                error.type_ = TypeError{"Can't append value to type that isn't list.", node.nodes.front().atom.line};
                return Null();
            }
            obj.list.push_back(appender);
            // std::cout << appender.value << std::endl;
            return obj;

        }
        

        else if (node.nodes.front().atom.value == "push") {
            if ( (int) node.nodes.size() <3) {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"Insufficent number of arguments passed for 'push'.", node.nodes.front().atom.line};
            }
            else if ((int) node.nodes.size() > 3) {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"Too many arguments passed for 'push'.", node.nodes.front().atom.line};
            }

            object obj = evaluate(node.nodes.at(2));
            if (error_found) return Null();
            object appender = evaluate(node.nodes.at(1));
            if (error_found) return Null();

            if (obj.type != BOP_LIST) {
                error_found = true;
                error.type = TYPE_ERROR;
                error.type_ = TypeError{"Can't push value to type that isn't list.", node.nodes.front().atom.line};
                return Null();
            }
            obj.list.insert(obj.list.begin(), appender);
            // std::cout << appender.value << std::endl;
            return obj;
        }


        else {
            error_found = true;
            error.type = KEYWORD_ERROR;
            error.keyword = KeywordError{"Unrecogonized keyword '" + node.nodes.front().atom.value + "'.", 
                node.nodes.front().atom.line};
        }
    }
    return Null();
}