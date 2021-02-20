//#define TYPE_ERASURE_HEAP_QUEUE   // choose heap implementation
#define TYPE_PARAM_HEAP_QUEUE
//#define TYPE_PARAM_VECTOR_QUEUE
//#define TYPE_ERASURE_VECTOR_QUEUE
#include "priority_queue.hpp"
#include <iostream>

#define PRINT(s)      std::cout << (s)
#define PRINTLN(s)    PRINT(s) << std::endl

int main(int argc, char **argv)
{
    //PriorityQueue<int> pqi([](const int &a, const int &b) { return a > b; });   // type erasure based implementation
    PriorityQueue<int, decltype(&Less<int>)> pqi(&Less<int>);                     // template type param based implementation
    //PriorityQueue<int> pqi;

    pqi.Insert(0);
    pqi.Insert(0);
    pqi.Insert(0);
    pqi.Insert(22);
    pqi.Insert(31);
    pqi.Insert(41);
    pqi.Insert(11);
    pqi.Insert(10);
    pqi.Insert(41);

    size_t size = pqi.Size();

    for (size_t i = 0; i < size; i++)
    {
        PRINTLN(pqi.Peek());
        pqi.Remove();
    }

    return 0;
}