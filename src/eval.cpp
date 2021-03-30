#include "../include/eval.h"

Node AST_Handler::next() 
{
    ptr++;
    if (ptr >= (int) ast.nodes.size()) 
    {
        eof = true;
    }
    return ast.nodes.at(ptr-1);
}

object Null() 
{
    return make_object(BOP_NUMBER, "0");
}

// formatting strings
std::string Function_format_string(std::vector<object> objs) 
{
    std::string res;
    for (int i = 0; i < (int)objs.size(); i++) 
    {
        res += repr(objs.at(i));
    }
    return "'"+res+"'";
}


// Math stuff #################################################################
std::string Function_add_nums(std::vector<object> objs) 
{
    double res = to_number(objs.at(0).value);
    for (int i = 1; i < (int)objs.size(); i++) 
    {
        res += to_number(objs.at(i).value);
    }
    return std::to_string(res);
}
std::string Function_subtract_nums(std::vector<object> objs) 
{
    double res = to_number(objs.at(0).value);
    for (int i = 1; i < (int)objs.size(); i++) 
    {
        res -= to_number(objs.at(i).value);
    }
    return std::to_string(res);
}
std::string Function_times_nums(std::vector<object> objs) 
{
    double res = to_number(objs.at(0).value);
    for (int i = 1; i < (int)objs.size(); i++) 
    {
        res *= to_number(objs.at(i).value);
    }
    return std::to_string(res);
}
std::string Function_div_nums(std::vector<object> objs) 
{
    double res = to_number(objs.at(0).value);
    for (int i = 1; i < (int)objs.size(); i++) 
    {
        res /= to_number(objs.at(i).value);
    }
    return std::to_string(res);
}
// ############################################################################


// Evaluate ###################################################################
void Evaluator::destructive_return(std::string name, object value)
{
    Env top = env_stack.back();
    top.set_key(name, value);
    env_stack.pop();
    env_stack.push(top);
}


object Evaluator::evaluate(Node node) 
{
    if (node.type == ATOM) 
    {
        Atom atom = node.atom;
        if (atom.type == NUMBER)
        {
            return make_object(BOP_NUMBER, atom.value);
        }
        else if (atom.type == STRING)
        {
            return make_object(BOP_STRING, atom.value); 
        }
        else 
        {
            if (env_stack.back().has_key(atom.value)) 
            {
                return env_stack.back().at_key(atom.value); 
            }
            else 
            {
                error_found = true;
                error.type = KEYWORD_ERROR;
                error.keyword = KeywordError{"Undefined variable '" + atom.value + "'.", atom.line};
                return Null();
            }
        }
    } 
    else
    {
        if (node.nodes.front().atom.value == "println") 
        {
            if ((int) node.nodes.size() > 2) 
            {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"Too many arguments passed for 'println'.", node.nodes.front().atom.line};
            } 
            else if ((int) node.nodes.size() == 1) 
            {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"No arguments passed for 'println'.", node.nodes.front().atom.line};
            }
            object obj = evaluate(node.nodes.back());
            if (error_found) 
            {
                return Null();
            }
            std::cout << repr(obj) << std::endl;
            return Null();
        } 
        
        else if (node.nodes.front().atom.value == "print") 
        {
            if ((int) node.nodes.size() > 2) 
            {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"Too many arguments passed for 'print'.", node.nodes.front().atom.line};
            } 
            else if ((int) node.nodes.size() == 1) 
            {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"No arguments passed for 'print'.", node.nodes.front().atom.line};
            }
            object obj = evaluate(node.nodes.back());
            if (error_found) 
            {
                return Null();
            }
            std::cout << repr(obj);
            return Null();
        } 
        
        else if (node.nodes.front().atom.value == "let") 
        {
            if ((int) node.nodes.size() > 3) 
            {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"Too many arguments passed for 'let'.", node.nodes.front().atom.line};
            } 
            else if ((int) node.nodes.size() < 3) 
            {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"Insufficient number of arguments passed for 'let'.", node.nodes.front().atom.line};
            }
            object obj = evaluate(node.nodes.back());
            if (error_found) 
            {
                return Null();
            }
            if (node.nodes.at(1).atom.type != SYMBOL) 
            {
                error_found = true;
                error.type = KEYWORD_ERROR;
                error.keyword = KeywordError{"Can't set variable name to non-symbol type.", node.nodes.front().atom.line};
                return Null();
            }
            Env env = env_stack.back();
            env.set_key(node.nodes.at(1).atom.value, obj);
            env_stack.pop();
            env_stack.push(env);
            return Null();
        }

        else if (node.nodes.front().atom.value == "format") 
        {
            if ((int) node.nodes.size() == 1) 
            {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"No arguments passed for 'format'.", node.nodes.front().atom.line};
            }
            std::vector<object> formatters;
            for (int i = 1; i < (int) node.nodes.size(); i++) 
            {
                Node n = node.nodes.at(i);
                formatters.push_back(evaluate(n));
                if (error_found) 
                {
                    return Null();
                }
            }
            return make_object(BOP_STRING, Function_format_string(formatters));
        }
        
        else if (node.nodes.front().atom.value == "+") 
        {
            if ( (int) node.nodes.size() == 1) 
            {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"No arguments passed for '+'.", node.nodes.front().atom.line};
                return Null();
            }
            std::vector<object> formatters;
            for (int i = 1; i < (int) node.nodes.size(); i++) 
            {
                Node n = node.nodes.at(i);
                object o = evaluate(n);
                if (error_found) 
                {
                    return Null();
                }
                if (o.type != BOP_NUMBER) 
                {
                    error_found = true;
                    error.type = TYPE_ERROR;
                    error.type_ = TypeError{"Can't add with non-numeric type.", node.nodes.front().atom.line};
                    return Null();
                }
                formatters.push_back(o);
            }
            return make_object(BOP_NUMBER, Function_add_nums(formatters));
        }

        else if (node.nodes.front().atom.value == "-") 
        {
            if ((int) node.nodes.size() == 1) 
            {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"No arguments passed for '-'.", node.nodes.front().atom.line};
                return Null();
            }
            std::vector<object> formatters;
            for (int i = 1; i < (int) node.nodes.size(); i++) 
            {
                Node n = node.nodes.at(i);
                object o = evaluate(n);
                if (error_found) 
                {
                    return Null();
                }
                if (o.type != BOP_NUMBER) 
                {
                    error_found = true;
                    error.type = TYPE_ERROR;
                    error.type_ = TypeError{"Can't subtract with non-numeric type.", node.nodes.front().atom.line};
                    return Null();
                }
                formatters.push_back(o);
            }
            return make_object(BOP_NUMBER, Function_subtract_nums(formatters));
        }

        else if (node.nodes.front().atom.value == "*") 
        {
            if ((int) node.nodes.size() == 1) 
            {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"No arguments passed for '*'.", node.nodes.front().atom.line};
                return Null();
            }
            std::vector<object> formatters;
            for (int i = 1; i < (int) node.nodes.size(); i++) 
            {
                Node n = node.nodes.at(i);
                object o = evaluate(n);
                if (error_found) 
                {
                    return Null();
                }
                if (o.type != BOP_NUMBER) 
                {
                    error_found = true;
                    error.type = TYPE_ERROR;
                    error.type_ = TypeError{"Can't multiply with non-numeric type.", node.nodes.front().atom.line};
                }
                formatters.push_back(o);
            }
            return make_object(BOP_NUMBER, Function_times_nums(formatters));
        }

        else if (node.nodes.front().atom.value == "/") 
        {
            if ((int) node.nodes.size() == 1) 
            {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"No arguments passed for '/'.", node.nodes.front().atom.line};
                return Null();
            }
            std::vector<object> formatters;
            for (int i = 1; i < (int) node.nodes.size(); i++) 
            {
                Node n = node.nodes.at(i);
                object o = evaluate(n);
                if (error_found) 
                {
                    return Null();
                }
                if (o.type != BOP_NUMBER) 
                {
                    error_found = true;
                    error.type = TYPE_ERROR;
                    error.type_ = TypeError{"Can't divide with non-numeric type.", node.nodes.front().atom.line};
                    return Null();
                } 
                else if (o.value == "0") 
                {
                    error_found = true;
                    error.type = DIVISION_BY_ZERO_ERROR;
                    error.division = DivisionByZeroError{node.nodes.front().atom.line};
                    return Null();
                }
                formatters.push_back(o);
            }
            return make_object(BOP_NUMBER, Function_div_nums(formatters));
        }

        else if (node.nodes.front().atom.value == "list") 
        {
            if ((int)node.nodes.size() == 1) 
            {
                return make_object(BOP_LIST, "");
            }
            else 
            {
                std::vector<object> formatters;
                for (int i = 1; i < (int) node.nodes.size(); i++) 
                {
                    Node n = node.nodes.at(i);
                    object o = evaluate(n);
                    formatters.push_back(o);
                    if (error_found) 
                    {
                        return Null();
                    }
                }
                return make_object(BOP_LIST, "", formatters);
            }
        }
        
        else if (node.nodes.front().atom.value == "first") 
        {
            if ( (int) node.nodes.size() == 1) 
            {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"No arguments passed for 'first'.", node.nodes.front().atom.line};
                return Null();
            }
            else if ((int) node.nodes.size() > 2) 
            {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"Too many arguments passed for 'first'.", node.nodes.front().atom.line};
                return Null();
            }
            object obj = evaluate(node.nodes.at(1));
            if (error_found) 
            {
                return Null();
            }
            if (obj.type != BOP_LIST and obj.type != BOP_STRING) 
            {
                error_found = true;
                error.type = TYPE_ERROR;
                error.type_ = TypeError{"Can't get first value on type that isn't string or list.", node.nodes.front().atom.line};
                return Null();
            }
            if (obj.type == BOP_LIST)
            {
                if ((int)obj.list.size() == 0) 
                {
                    error_found = true;
                    error.type = INDEX_ERROR;
                    error.index = IndexError{"Can't get first value from empty list.", node.nodes.front().atom.line};
                    return Null();
                }
                return obj.list.front();
            }
            else 
            {
                if ((int)obj.value.length() == 0) 
                {
                    error_found = true;
                    error.type = INDEX_ERROR;
                    error.index = IndexError{"Can't get first value from empty string.", node.nodes.front().atom.line};
                    return Null();
                }
                return make_object(BOP_STRING, "'"+repr(obj).substr(0, 1)+"'"); 
            }
        }
        
        else if (node.nodes.front().atom.value == "last") 
        {
            if ((int) node.nodes.size() == 1) 
            {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"No arguments passed for last'.", node.nodes.front().atom.line};
                return Null();
            }
            else if ((int) node.nodes.size() > 2) 
            {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"Too many arguments passed for 'last'.", node.nodes.front().atom.line};
                return Null();
            }
            object obj = evaluate(node.nodes.at(1));
            if (error_found)
            {
                return Null();
            }
            if (obj.type != BOP_LIST and obj.type != BOP_STRING) 
            {
                error_found = true;
                error.type = TYPE_ERROR;
                error.type_ = TypeError{"Can't get last value on type that isn't string or list.", node.nodes.front().atom.line};
                return Null();
            }
            if (obj.type == BOP_LIST) 
            {
                if ((int)obj.list.size() == 0) 
                {
                    error_found = true;
                    error.type = INDEX_ERROR;
                    error.index = IndexError{"Can't get last value from empty list.", node.nodes.front().atom.line};
                    return Null();
                }
                return obj.list.back();
            } 
            else 
            {
                if ((int)obj.value.length() == 0) 
                {
                    error_found = true;
                    error.type = INDEX_ERROR;
                    error.index = IndexError{"Can't get last value from empty string.", node.nodes.front().atom.line};
                    return Null();
                }
                std::string c = "'";
                c.push_back(repr(obj).back());
                c.push_back('\'');
                return make_object(BOP_STRING, c); 
            }
        }
        
        else if (node.nodes.front().atom.value == "rest") 
        {
            if ((int) node.nodes.size() == 1) 
            {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"No arguments passed for 'rest'.", node.nodes.front().atom.line};
                return Null();
            }
            else if ((int) node.nodes.size() > 2) 
            {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"Too many arguments passed for 'rest'.", node.nodes.front().atom.line};
                return Null();
            }
            object obj = evaluate(node.nodes.at(1));
            if (error_found) 
            {
                return Null();
            }
            if (obj.type != BOP_LIST and obj.type != BOP_STRING) 
            {
                error_found = true;
                error.type = TYPE_ERROR;
                error.type_ = TypeError{"Can't get values on type that isn't string or list.", node.nodes.front().atom.line};
                return Null();
            }
            if (obj.type == BOP_LIST) 
            {
                if ((int)obj.list.size() == 0) 
                {
                    error_found = true;
                    error.type = INDEX_ERROR;
                    error.index = IndexError{"Can't get the rest of a list that is empty.", node.nodes.front().atom.line};
                    return Null();
                }
                std::vector<object> sub_list;
                for (int i = 1; i < obj.list.size(); i++) 
                {
                    sub_list.push_back(obj.list.at(i));
                }
                return make_object(BOP_LIST, "", sub_list);
            }
            else 
            {
                if ((int)obj.value.length() == 0) 
                {
                    error_found = true;
                    error.type = INDEX_ERROR;
                    error.index = IndexError{"Can't get the rest of a  string that is empty.", node.nodes.front().atom.line};
                    return Null();
                }
                return make_object(BOP_STRING, "'"+repr(obj).substr(1)+"'"); 
            }
        }
        
        else if (node.nodes.front().atom.value == "nth") 
        {
            if ((int) node.nodes.size() < 3) 
            {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"Insufficent number of arguments passed for 'nth'.", node.nodes.front().atom.line};
                return Null();
            }
            else if ((int) node.nodes.size() > 3) 
            {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"Too many arguments passed for 'nth'.", node.nodes.front().atom.line};
                return Null();
            }

            object obj = evaluate(node.nodes.at(1));
            if (error_found)
            {
                return Null();
            }
            object idx = evaluate(node.nodes.at(2));
            if (error_found) 
            {
                return Null();
            }

            if (obj.type != BOP_LIST and obj.type != BOP_STRING) 
            {
                error_found = true;
                error.type = TYPE_ERROR;
                error.type_ = TypeError{"Can't get value from type that isn't string or list.", node.nodes.front().atom.line};
                return Null();
            }
            if (idx.type != BOP_NUMBER) 
            {
                error_found = true;
                error.type = TYPE_ERROR;
                error.type_ = TypeError{"Can't get use value that isn't number as index.", node.nodes.front().atom.line};
                return Null();
            } 
            
            if (obj.type == BOP_LIST) 
            {
                if (atoi(idx.value.c_str()) > (int)obj.list.size()-1) 
                {
                    error_found = true;
                    error.type = INDEX_ERROR;
                    error.index = IndexError{"Index " + idx.value + " out of range.", node.nodes.front().atom.line};
                    return Null();
                }
                return obj.list.at(atoi(idx.value.c_str()));
            } 
            else if (obj.type == BOP_STRING) 
            {
                if (atoi(idx.value.c_str()) > (int)repr(obj).length()-1) 
                {
                    error_found = true;
                    error.type = INDEX_ERROR;
                    error.index = IndexError{"Index " + idx.value + " out of range.", node.nodes.front().atom.line};
                    return Null();
                }
                int idx_ = atoi(idx.value.c_str());
                std::string str = "'"; 
                str.push_back(repr(obj).at(idx_));
                str.push_back('\'');
                return make_object(BOP_STRING, str);
            }
        }
        
        else if (node.nodes.front().atom.value == "append") 
        {
            if ((int) node.nodes.size() < 3) 
            {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"Insufficent number of arguments passed for 'append'.", node.nodes.front().atom.line};
                return Null();
            }
            else if ((int) node.nodes.size() > 3) 
            {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"Too many arguments passed for 'append'.", node.nodes.front().atom.line};
                return Null();
            }
            object appender = evaluate(node.nodes.at(1));
            if (error_found)
            {
                return Null();
            }
            object obj = evaluate(node.nodes.at(2));
            if (error_found)
            {
                return Null();
            }
            if (obj.type != BOP_LIST) 
            {
                error_found = true;
                error.type = TYPE_ERROR;
                error.type_ = TypeError{"Can't append value to type that isn't list.", node.nodes.front().atom.line};
                return Null();
            }
            obj.list.push_back(appender);
            // std::cout << appender.value << std::endl;
            if (node.nodes.at(2).type == ATOM)
            {
                destructive_return(node.nodes.at(2).atom.value, obj);
            }
            return obj;
        }
        
        else if (node.nodes.front().atom.value == "push") 
        {
            if ((int) node.nodes.size() < 3) 
            {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"Insufficent number of arguments passed for 'push'.", node.nodes.front().atom.line};
                return Null();
            }
            else if ((int) node.nodes.size() > 3) 
            {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"Too many arguments passed for 'push'.", node.nodes.front().atom.line};
                return Null();
            }
            object appender = evaluate(node.nodes.at(1));
            if (error_found)
            {
                return Null();
            }
            object obj = evaluate(node.nodes.at(2));
            if (error_found)
            {
                return Null();
            }

            if (obj.type != BOP_LIST) 
            {
                error_found = true;
                error.type = TYPE_ERROR;
                error.type_ = TypeError{"Can't push value to type that isn't list.", node.nodes.front().atom.line};
                return Null();
            }
            obj.list.insert(obj.list.begin(), appender);
            // std::cout << appender.value << std::endl;
            if (node.nodes.at(2).type == ATOM)
            {
                destructive_return(node.nodes.at(2).atom.value, obj);
            }
            return obj;
        }

        else if (node.nodes.front().atom.value == "sub")
        {
            if ((int) node.nodes.size() < 3) 
            {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"Insufficent number of arguments passed for 'sub'.", node.nodes.front().atom.line};
                return Null();
            }
            else if ((int) node.nodes.size() > 4) 
            {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"Too many arguments passed for 'sub'.", node.nodes.front().atom.line};
                return Null();
            }
            object obj = evaluate(node.nodes.at(1));
            if (error_found)
            {
                return Null();
            }
            if (obj.type == BOP_LIST) 
            {
                object idx_start = evaluate(node.nodes.at(2));
                if (error_found)
                {
                    return Null();
                }
                if (idx_start.type != BOP_NUMBER)
                {
                    error_found = true;
                    error.type = TYPE_ERROR;
                    error.type_ = TypeError{"Can't use value that isn't a number as the start index.", node.nodes.front().atom.line};
                    return Null();
                }
                if (to_number(idx_start.value) > (int)obj.list.size() or to_number(idx_start.value) < 0)
                {
                    error_found = true;
                    error.type = INDEX_ERROR;
                    error.index = IndexError{"Index " + idx_start.value + " out of range.", node.nodes.front().atom.line};
                    return Null();
                }
                if ((int)node.nodes.size() == 3)
                {
                    std::vector<object> sublist(obj.list.begin() + to_number(idx_start.value), obj.list.end());
                    return make_object(BOP_LIST, "", sublist);
                }
                else 
                {
                    object idx_end = evaluate(node.nodes.at(3));
                    if (error_found)
                    {
                        return Null();
                    }
                    if (idx_start.type != BOP_NUMBER)
                    {
                        error_found = true;
                        error.type = TYPE_ERROR;
                        error.type_ = TypeError{"Can't use value that isn't a number as the end index.", node.nodes.front().atom.line};
                        return Null();
                    }
                    if (to_number(idx_end.value) > (int)obj.list.size() or to_number(idx_end.value) < 0)
                    {
                        error_found = true;
                        error.type = INDEX_ERROR;
                        error.index = IndexError{"Index " + idx_end.value + " out of range.", node.nodes.front().atom.line};
                        return Null();
                    }
                    std::vector<object> sublist(obj.list.begin() + to_number(idx_start.value), obj.list.begin()+to_number(idx_end.value));
                    return make_object(BOP_LIST, "", sublist);
                }
            }
            else if (obj.type == BOP_STRING)
            {
                object idx_start = evaluate(node.nodes.at(2));
                if (error_found)
                {
                    return Null();
                }
                if (idx_start.type != BOP_NUMBER)
                {
                    error_found = true;
                    error.type = TYPE_ERROR;
                    error.type_ = TypeError{"Can't use value that isn't a number as the start index.", node.nodes.front().atom.line};
                    return Null();
                }
                if (to_number(idx_start.value) > (int)obj.value.length() or to_number(idx_start.value) < 0)
                {
                    error_found = true;
                    error.type = INDEX_ERROR;
                    error.index = IndexError{"Index " + idx_start.value + " out of range.", node.nodes.front().atom.line};
                    return Null();
                }
                if ((int)node.nodes.size() == 3)
                {
                    return make_object(BOP_STRING, repr(obj).substr(to_number(idx_start.value)));
                }
                else 
                {
                    object idx_end = evaluate(node.nodes.at(3));
                    if (error_found)
                    {
                        return Null();
                    }
                    if (idx_end.type != BOP_NUMBER)
                    {
                        error_found = true;
                        error.type = TYPE_ERROR;
                        error.type_ = TypeError{"Can't use value that isn't a number as the end index.", node.nodes.front().atom.line};
                        return Null();
                    }
                    if (to_number(idx_end.value) > (int)obj.value.length() or to_number(idx_end.value) < 0)
                    {
                        error_found = true;
                        error.type = INDEX_ERROR;
                        error.index = IndexError{"Index " + idx_end.value + " out of range.", node.nodes.front().atom.line};
                        return Null();
                    }
                    return make_object(BOP_STRING, "'"+repr(obj).substr(to_number(idx_start.value), to_number(idx_end.value))+"'");
                }
            }
            else 
            {
                error_found = true;
                error.type = TYPE_ERROR;
                error.type_ = TypeError{"Can't get subvalue of value that isn't of type list or string.", node.nodes.front().atom.line};
                return Null();
            }
        }

        else if (node.nodes.front().atom.value == "len")
        {
            if ((int) node.nodes.size() == 1) 
            {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"No arguments passed for 'len'.", node.nodes.front().atom.line};
                return Null();
            }
            else if ((int) node.nodes.size() > 2) 
            {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"Too many arguments passed for 'len'.", node.nodes.front().atom.line};
                return Null();
            }
            object obj = evaluate(node.nodes.at(1));
            if (error_found)
            {
                return Null();
            }
            if (obj.type == BOP_LIST)
            {
                return make_object(BOP_NUMBER, std::to_string(obj.list.size()));
            }
            else if (obj.type == BOP_STRING)
            {
                return make_object(BOP_NUMBER, std::to_string(repr(obj).length()));
            }
            else 
            {
                error_found = true;
                error.type = TYPE_ERROR;
                error.type_ = TypeError{"Can't get length from type that isn't string or list.", node.nodes.front().atom.line};
                return Null();
            }
        }

        else if (node.nodes.front().atom.value == "setf")
        {
            if ((int) node.nodes.size() < 4) 
            {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"Insufficent number of arguments passed for 'setf'.", node.nodes.front().atom.line};
                return Null();
            }
            else if ((int) node.nodes.size() > 4) 
            {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"Too many arguments passed for 'setf'.", node.nodes.front().atom.line};
                return Null();
            }
        }

        else if (node.nodes.front().atom.value == "read") 
        {
            if ((int) node.nodes.size() > 2) 
            {
                error_found = true;
                error.type = ARGUMENT_ERROR;
                error.arg = ArgumentError{"Too many arguments passed for 'read'.", node.nodes.front().atom.line};
                return Null();
            } 
            else if ((int) node.nodes.size() == 1) 
            {
                std::string input;
                getline(std::cin, input);
                return make_object(BOP_STRING, "'"+input+"'");
            }
            object obj = evaluate(node.nodes.back());
            if (error_found)
            {
                return Null();
            }
            std::cout << repr(obj);
            std::string input;
            getline(std::cin, input);
            return make_object(BOP_STRING, "'"+input+"'");
        } 

        else 
        {
            error_found = true;
            error.type = KEYWORD_ERROR;
            error.keyword = KeywordError{"Unrecogonized keyword '" + node.nodes.front().atom.value + "'.", node.nodes.front().atom.line};
        }
    }
    return Null();
}
// ############################################################################