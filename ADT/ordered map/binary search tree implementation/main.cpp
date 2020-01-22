//#include "map_erasure.hpp"
#include "map_parameter.hpp"
#include <string>
#include <iostream>

#define PRINT(s)     std::cout << (s)
#define PRINTLN(s)   PRINT(s) << std::endl

int main(int argc, char **argv)
{
    Map<std::string,int> t;

    t.Insert("A", 32);
    t.Insert("B", 65);
    t.Insert("C", 2);
    t.Insert("D", 11);
    t.Insert("E", 120);
    t.Insert("F", 1);

    auto it = t.Find("F");

    if (it != t.End())
    {
        PRINT("found: ");
        PRINT(it->Key()); PRINT(",");
        PRINTLN(it->Value());
    }

    it = t.Find("Z");

    if (it == t.End())
        PRINTLN("not found!");
    else
        PRINTLN("found!");

    for (auto i = t.Begin(); i != t.End(); ++i)
    {
        PRINT(i->Key()); PRINT(",");
        PRINTLN(i->Value());
    }

    PRINTLN("*****");

    auto it2 = t.Find("D");

    do 
    {
        PRINT(it2->Key()); PRINT(",");
        PRINTLN(it2->Value());
        it2;
    }
    while (it2-- != t.Begin());

    t.Remove("B");
    t.Remove("C");
    t.Remove("E");
    t.Remove(t.CBegin());

    PRINTLN("");

    for (auto it = t.Begin(); it != t.End(); ++it)
    {
        PRINT(it->Key()); PRINT(",");
        PRINTLN(it->Value());
    }

    return 0;
}