//#include "sorted_list_erasure.hpp"
#include "sorted_list_parameter.hpp"
//#include "../../list/singly_linked_list.hpp"
//#include "../../list/double_ended_doubly_linked_list.hpp"
#include "../../list/double_ended_singly_linked_list.hpp"
#include <string>
#include <iostream>

#define PRINT(s)     std::cout << (s)
#define PRINTLN(s)   PRINT(s) << std::endl

int main(int argc, char *argv[])
{
    SortedList<std::string> sls;  // default vector implementation

    sls.Insert("hello");
    sls.Insert("world");
    sls.Insert("once");
    sls.Insert("upon");
    sls.Insert("a");
    sls.Insert("time");

    auto it = sls.CBegin();

    while (it != sls.End())
        PRINTLN(*it++);
    
    PRINTLN("");
    
    //SortedList<std::string, DoublyLinkedList> sls2;   // linked list implementation
    SortedList<std::string, decltype(&Less<std::string>), DE_SinglyLinkedList> sls2;   // linked list implementation for parameter comparator

    sls2.Insert("hello");
    sls2.Insert("world");
    sls2.Insert("once");
    sls2.Insert("upon");
    sls2.Insert("a");
    sls2.Insert("time");

    auto it2 = sls2.CBegin();

    while (it2 != sls2.CEnd())
        PRINTLN(*it2++);
    
    PRINTLN("");

    return 0;
}