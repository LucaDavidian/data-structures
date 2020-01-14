#include "deque.hpp"
#include <string>
#include <iostream>

#include "../../list/singly_linked_list.hpp"
#include "../../array/circular_array.hpp"

#define PRINT(s)       std::cout << (s)
#define PRINTLN(s)     PRINT(s) << std::endl;

int main(int argc, char **argv)
{
    Deque<std::string> ds;   // double ended doubly linked list implementation (default)

    ds.InsertBack("world");
    ds.InsertBack("!");
    ds.InsertFront("hello");
    ds.InsertFront("says: ");
    ds.InsertFront("Luca ");
    
    while (!ds.Empty())
    {
        PRINTLN(ds.Front());
        ds.RemoveFront();
    }

    ds.InsertBack("world");
    ds.InsertBack("!");
    ds.InsertFront("hello");
    ds.InsertFront("says: ");
    ds.InsertFront("Luca ");

    PRINT("removed: "); PRINTLN(ds.Back());
    ds.RemoveBack();

    PRINT("removed: "); PRINTLN(ds.Back());
    ds.RemoveBack();

    while (!ds.Empty())
    {
        PRINTLN(ds.Front());
        ds.RemoveFront();
    }

    PRINTLN("***********************************");

    Deque<std::string,SinglyLinkedList> ds2;  // singly linked list implementation

    ds2.InsertBack("world");
    ds2.InsertBack("!");
    ds2.InsertFront("hello");
    ds2.InsertFront("says: ");
    ds2.InsertFront("Luca ");
    
    while (!ds2.Empty())
    {
        PRINTLN(ds2.Front());
        ds2.RemoveFront();
    }

    ds2.InsertBack("world");
    ds2.InsertBack("!");
    ds2.InsertFront("hello");
    ds2.InsertFront("says: ");
    ds2.InsertFront("Luca ");

    PRINT("removed: "); PRINTLN(ds2.Back());
    ds2.RemoveBack();

    PRINT("removed: "); PRINTLN(ds2.Back());
    ds2.RemoveBack();

    while (!ds2.Empty())
    {
        PRINTLN(ds2.Front());
        ds2.RemoveFront();
    }
   
    PRINTLN("***********************************");

    Deque<std::string,CircularArray> ca;  // ring buffer implementation

    ca.InsertBack("world");
    ca.InsertBack("!");
    ca.InsertFront("hello");
    ca.InsertFront("says: ");
    ca.InsertFront("Luca ");
    
    while (!ca.Empty())
    {
        PRINTLN(ca.Front());
        ca.RemoveFront();
    }

    ca.InsertBack("world");
    ca.InsertBack("!");
    ca.InsertFront("hello");
    ca.InsertFront("says: ");
    ca.InsertFront("Luca ");

    PRINT("removed: "); PRINTLN(ca.Back());
    ca.RemoveBack();

    PRINT("removed: "); PRINTLN(ca.Back());
    ca.RemoveBack();

    while (!ca.Empty())
    {
        PRINTLN(ca.Front());
        ca.RemoveFront();
    }


    return 0;
}