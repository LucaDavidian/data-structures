#include "stack.hpp"
#include <string>
#include <iostream>

#include "../../list/double_ended_doubly_linked_list.hpp"

#define PRINT(s)    std::cout << (s)
#define PRINTLN(s)  PRINT(s) << std::endl

int main(int argc, char **argv)
{
    // Stack class uses InsertLast and RemoveLast: 
    // in default vector implementation InsertLast and RemoveLast are both O(1)
    Stack<int> si;   // vector implementation (default)

    si.Push(10);
    si.Push(1);
    si.Push(120);
    si.Push(3);

    while (!si.Empty())
    {
        PRINT(si.Top()); 
        si.Pop();
        PRINT(" ");
    }

    PRINTLN("");
    PRINTLN("**********************************");

    // Stack class uses InsertLast and RemoveLast: 
    // in a doubly-linked list both operations are O(1) constant-time
    // in singly linked list InsertLast is O(1) if double ended, but RemoveLast is always O(n)
    Stack<std::string, DoublyLinkedList> ss;  // doubly linked list implementation

    ss.Push("!");
    ss.Push("world");
    ss.Push("hello");

    while (!ss.Empty())
    {
        PRINT(ss.Top());
        ss.Pop();
        PRINT(" ");
    }

    return 0;
}