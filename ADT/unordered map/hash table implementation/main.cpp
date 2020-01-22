#include "unordered map.hpp"
#include <string>
#include <iostream>

#define PRINT(s)     std::cout << (s)
#define PRINTLN(s)   PRINT(s) << std::endl

int main(int argc, char *argv[])
{
    Map<std::string, int> ms(100);

    ms.Insert("hello", 10);
    ms.Insert(std::string("world"), 11);
    ms.Insert("hey", 12);
    ms.Insert("!!!", 13);

    auto it = ms.Begin();

    while (it != ms.End())
    {
        PRINT("key: "); PRINT(it->Key()); PRINT(" value: "); PRINT(it->Value());
        PRINTLN("");
        ++it;
    }

    ms.Remove(ms.Find("!!!"));

    it = ms.Begin();

    while (it != ms.End())
    {
        PRINT("key: "); PRINT(it->Key()); PRINT(" value: "); PRINT(it->Value());
        PRINTLN("");
        ++it;
    }

    ms.Insert(Entry<std::string,int>("once upon a time", 100));
    Entry<std::string, int> e("conversion", 1.29f);
    ms.Insert(e);

    it = ms.Begin();

    while (it != ms.End())
    {
        PRINT("key: "); PRINT(it->Key()); PRINT(" value: "); PRINT(it->Value());
        PRINTLN("");
        ++it;
    }

    return 0;
}