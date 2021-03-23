#include <vector>
#include <string>
#include <iostream>

// Slightly modified Env struct 
struct test_env
{
    std::vector<std::string> keys;
    std::vector<std::string> values;

    bool has_key(std::string key) {
        for (int i = 0; i < (int) keys.size(); i++) {
            std::string key_comp = keys.at(i);
            if (key_comp == key) return true;
        }
        return false;
    }

    void add_key(std::string key, std::string value) {
        keys.push_back(key);
        values.push_back(value);
    }
    
    void set_key(std::string key, std::string value) {
        if(has_key(key)) {
            for (int i = 0; i < (int) keys.size(); i++) {
                std::string key_comp = keys.at(i);
                if (key_comp == key) {
                    values[i] = value; return;
                }
            }
        } else add_key(key, value);
    }

    void print() {
        for (int i = 0; i < (int) keys.size(); i++) {
            std::string key = keys.at(i);
            std::string value = values.at(i);
            std::cout << key << "  :  " << value << std::endl;
        }
    }
};



int main(int argc, char const *argv[])
{
    test_env env;
    env.set_key("Hello", " world!");
    env.set_key("My Second Key", "This value!");
    env.print();
    env.set_key("My Second Key", "New value!");
    env.print();
    return 0;
}
