#include "circular_array.hpp"
#include "ring_buffer.hpp"
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


    RingBuffer<int, 5> rb;

    PRINT("ring buffer size: "); PRINTLN(rb.Size());

    rb.InsertLast(1);
    rb.InsertLast(2);
    rb.InsertLast(3);
    rb.InsertLast(4);
    rb.InsertFirst(0);

    while (!rb.Empty())
    {      
        PRINTLN(rb.First());
        rb.RemoveFirst();
    }

    try 
    {
        rb.RemoveLast();
    }
    catch (std::exception)
    {
        PRINT("empty: exception caught"); 
    }

    return 0;
}