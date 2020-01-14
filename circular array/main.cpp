#include "circular_array.hpp"
#include <iostream>
#include <string>

#define PRINT(s)     std::cout << (s)
#define PRINTLN(s)   PRINT(s) << std::endl

int main(int argc, char *argv[])
{
    CircularArray<std::string> cs(5);

    cs.InsertFirst("hello");
    cs.InsertLast("world");
    cs.InsertLast("!");
    cs.InsertFirst("^");

    while (!cs.Empty())
    {
        PRINT("size is: "); PRINTLN(cs.Size());
        PRINT(cs.First());
        PRINT(" ");
        cs.RemoveFirst();
    }
    PRINT("size is: "); PRINTLN(cs.Size());

    try 
    {
        cs.Last();
    }
    catch (std::exception)
    {
        PRINT("exception caught"); 
    }

    return 0;
}