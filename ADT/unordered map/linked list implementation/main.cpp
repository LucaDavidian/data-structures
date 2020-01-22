#include "unordered_map.hpp"
#include <string>
#include <iostream>

#define PRINT(s)      std::cout << (s)
#define PRINTLN(s)    PRINT(s) << std::endl;

int main(int argc, char *argv[])
{
    Map<int, std::string> map;

    PRINTLN(map.Empty());
    PRINTLN(map.Size());

    map.Insert(1, "hello");
    map.Insert(2, "world");
    map.Insert(3, "!");

    Map<int,std::string>::Iterator it = map.Begin();
    
    while (it != map.End())
        PRINTLN(it++->Value());
    
    map.Insert(3, "!!!");

    it = map.Begin();

    while (it != map.End())
        PRINTLN(it++->Value());

    map.Remove(2);

    it = map.Begin();

    while (it != map.End())
        PRINTLN(it++->Value());

    return 0;
}