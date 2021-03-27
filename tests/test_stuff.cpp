#include <iostream>
#include <map>
#include <vector>

using std::cout; using std::cin;
using std::endl; using std::string;
using std::map; using std::copy;

template<typename Map>
void PrintMap(Map& m)
{
    cout << "[ ";
    for (auto &item : m) {
        cout << item.first << ":" << item.second << " ";
    }
    cout << "]\n";
}

int main() {
    map<std::string, string> map1 = {{"loop", "Apple",},
                                {"boop", "Banana",},
                                {"snoop", "Mango",},
                                {"coop", "Raspberry",},
                                {"Toop", "Blackberry",},
                                {"hehe", "Cocoa",}};

    cout << "map1 - ";
    PrintMap(map1);
    cout << endl;
    cout << map1.at("loop");
    cout << map1.at("snoop");

    map1.find("boop")->second = "NOOOO";
    cout << map1.at("boop");

    std::vector<int> list = {1, 2, 3, 4, 5, 6, 7, 8};

    std::vector<int> slice_of_x(list.begin() + 3, list.end());

    std::cout << slice_of_x.front() << " " << slice_of_x.back();

    return EXIT_SUCCESS;
}