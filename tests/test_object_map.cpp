#include <iostream>
#include <string>
#include <map>

/*
    Im debating whether i should use an object map.
    it would basically hold all the objects made by the user, and save them to an id.
    When you define a new variable like,
        (let x 1)
    It would increment the id, and set the id to the object inside the object map.
    For this example, it would set the id of 1 to the value 1.
    Then if i defined another variable like,
        (let y 4)
    it would set the id of 2 to the value 4.

    For now, this won't be a thing, but later, it might.
    This is for reference.
*/

class object
{
public:
    std::string value;
};


class object_map
{
public:
    std::map<int, object> map;
};


class env
{
public:
    object_map obj_map;
    std::map<std::string, int> env;
    int id = 0;

    void add_key(std::string value, object obj)
    {
        id++;
        env.insert(std::pair<std::string, int>(value, id));
        obj_map.map.insert(std::pair<int, object>(id, obj));
    }
    object get(std::string value)
    {
        int key = env.at(value);
        return obj_map.map.at(key);
    }
};


int main(int argc, char const *argv[])
{
    env Env;
    Env.add_key("MyVar", object{"My value"});
    object obj = Env.get("MyVar");
    std::cout << obj.value << std::endl;
    return 0;
}
