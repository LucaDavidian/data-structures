//#include "singly_linked_list.hpp"
//#include "double_ended_singly_linked_list.hpp"
#include "double_ended_doubly_linked_list.hpp"
#include "circularly_linked_list.hpp"

#include "../print_sequence.hpp"
#include "../list_reverse.hpp"
#include "../bubble_sort.hpp"

#include <iostream>

#define PRINT(s)     std::cout << (s)
#define PRINTLN(s)   PRINT(s) << std::endl

int main(int argc, char **argv)
{
    //SinglyLinkedList<int> li;
    //DE_SinglyLinkedList<int> li;
    DoublyLinkedList<int> li;

    li.InsertFirst(12);
    li.InsertFirst(1);
    li.InsertFirst(2);
    li.InsertFirst(10);
    li.InsertFirst(11);

    PrintSequence(li);
    PRINT("size is: "); PRINTLN(li.Size());

    li.RemoveFirst();
    li.RemoveLast();

    PrintSequence(li);
    PRINT("size is: "); PRINTLN(li.Size());

    li.Insert(li.Begin(), 111);
    li.Insert(li.End(), 222);

    PrintSequence(li);
    PRINT("size is: "); PRINTLN(li.Size());

    li.Remove(li.Find(12));
    li.Remove(li.Find(111));
    li.Remove(li.Find(222));

    PrintSequence(li);
    PRINT("size is: "); PRINTLN(li.Size());

    auto it = li.Begin();
    while (it != li.End())
    {
        if (*it == 10)
            it = li.Remove(it);
        else
        {
            PRINT(*it); PRINT(" ");
            ++it;
        }
    }
    PRINTLN("");
    
    PRINT("size is: "); PRINTLN(li.Size());

    li.Remove(li.Begin());
    li.Remove(li.Begin());

    try
    {
        li.Remove(li.Begin());
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    DoublyLinkedList<std::string> sl;

    sl.InsertFirst("world");
    sl.InsertFirst("hello");
    sl.InsertLast("!");
    sl.InsertLast("bye");

    sl.Insert(sl.Find("!"), "**");

    PrintSequence(sl);

    ListReverse(sl);

    PrintSequence(sl);

    BubbleSort(sl);

    PrintSequence(sl);

    DoublyLinkedList<std::string> newList = sl;

    PrintSequence(newList);

    DoublyLinkedList<std::string> newList2;

    newList2 = newList;

    PrintSequence(newList2);

    PRINTLN("");

    for (const std::string &element : newList2)
        PRINTLN(element); 

    PRINTLN("");

    /**** circularly linked list ****/

    CircularlyLinkedList<std::string> songs;

    songs.Insert("Eruption");
    songs.Insert("Dancing with myself");
    songs.Insert("Shout");
    songs.Insert("ABACAB");

    PRINTLN(songs.Back());
    PRINTLN(songs.Front());

    songs.Advance();

    PRINTLN(songs.Back());
    PRINTLN(songs.Front());

    songs.Remove();

    PRINTLN(songs.Back());
    PRINTLN(songs.Front());

    return 0;
}