#include "hash_table.hpp"
#include <string>
#include <iostream>

int main(int argc, char **argv)
{
    HashTable<std::string> ts(50);

    ts.Insert("hello");
    ts.Insert("world");
    ts.Insert("!!!!");
    ts.Insert("hey");

    auto it = ts.Begin();

    while (it != ts.End())
    {
        std::cout << "value: " << *it << std::endl;
        ++it;
    }
    std::cout << std::endl;

    ts.Remove(ts.Find("!!!!"));
    
    it = ts.Begin();

    while (it != ts.End())
    {
        std::cout << "value: " << *it << std::endl;
        ++it;
    }
    std::cout << std::endl;

    ts.Remove("hello");

    it = ts.Begin();

    while (it != ts.End())
    {
        std::cout << "value: " << *it << std::endl;
        ++it;
    }

    return 0;
}