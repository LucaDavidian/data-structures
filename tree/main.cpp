//#include "binary_search_tree_erasure.hpp"
#include "binary_search_tree_parameter.hpp"
#include <iostream>

#define PRINT(s)     std::cout << (s)
#define PRINTLN(s)   PRINT(s) << std::endl

int main(int argc, char *argv[])
{
    BinarySearchTree<int> t;

    t.InsertRecursive(10);
    t.InsertRecursive(20);
    t.InsertRecursive(8);
    t.InsertRecursive(0);
    t.InsertRecursive(11);
    t.InsertRecursive(112);
    t.InsertRecursive(1);
    t.InsertRecursive(1);
    t.InsertRecursive(54);

    auto it = t.Find(11);

    if (it != t.End())
    {
        PRINT("found: ");
        PRINTLN(*it);
    }

    it = t.Find(1);

    if (it == t.End())
        PRINTLN("not found!");
    else
        PRINTLN("found!");

    for (auto i = t.Begin(); i != t.End(); ++i)
        PRINTLN(*i);

    PRINTLN("*****");

    auto it2 = t.Find(112);

    do 
        PRINTLN(*it2--);
    while (it2 != t.Begin());

    t.Remove(10);
    t.Remove(11);
    t.Remove(112);
    t.Remove(1);
    t.Remove(t.CBegin());

    PRINTLN("");

    for (auto it = t.Begin(); it != t.End(); ++it)
        PRINTLN(*it);

    return 0;
}