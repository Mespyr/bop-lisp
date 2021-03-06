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

    float x = atof("12.00");
    float y = atof("44.00");
    float res = x+y;
    std::cout << std::endl << res << std::endl;

    std::string str = "Hello world";
    std::cout << str.substr(0, 5) << std::endl;
    std::vector<int> test_vec{0, 1, 2, 3, 4, 5, 6, 7};
    std::vector<int> sublist(test_vec.begin()+3, test_vec.begin()+6);
    std::cout << sublist.front() << " " << sublist.back() << std::endl;
    return EXIT_SUCCESS;
}