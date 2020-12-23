#include "queue.hpp"
#include <iostream>
#include <string>

#include "../../vector/vector.hpp"
#include "../../linked list/circularly_linked_list.hpp"
#include "../../circular array/circular_array.hpp"

#define PRINT(s)      std::cout << (s)
#define PRINTLN(s)    PRINT(s) << std::endl;

int main(int argc, char **argv)
{
    Queue<std::string> qs;  // double ended singly linked list implementation (default implementation)

    qs.Enqueue("hello");
    qs.Enqueue("world");
    qs.Enqueue("!");

    while (!qs.Empty())
    {
        PRINTLN(qs.Front());
        qs.Dequeue();
    }

    Queue<int, Vector> qi;  // vector implementation (Dequeue is O(n))

    qi.Enqueue(19);
    qi.Enqueue(29);
    qi.Enqueue(39);
    qi.Enqueue(49);
    qi.Enqueue(59);

    while (!qi.Empty())
    {
        PRINTLN(qi.Front());
        qi.Dequeue();
    }

    Queue<float,CircularlyLinkedList> cl;  // circularly linked list implementation

    cl.Enqueue(1.2f);
    cl.Enqueue(1.1f);
    cl.Enqueue(0.2f);
    cl.Enqueue(7.6f);
    cl.Enqueue(10.2f);

    while (!cl.Empty())
    {
        PRINTLN(cl.Front());
        cl.Dequeue();
    }

    Queue<float,CircularArray> ca;  // circular array implementation

    ca.Enqueue(13.2f);
    ca.Enqueue(31.1f);
    ca.Enqueue(30.2f);
    ca.Enqueue(73.6f);
    ca.Enqueue(130.2f);

    while (!ca.Empty())
    {
        PRINTLN(ca.Front());
        ca.Dequeue();
    }

    return 0;
}