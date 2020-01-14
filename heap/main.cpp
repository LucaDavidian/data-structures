#include <iostream>
#include <string>

#define PRINT(s)     std::cout << (s)
#define PRINTLN(s)   PRINT(s) << std::endl

//#define TYPE_ERASURE

#if defined TYPE_ERASURE  // heap class implementation uses type erasure for comparator 

    #include "heap_vector_erasure.hpp"

    int main(int argc, char **argv)
    {
        Heap<int> hi;

        hi.Insert(10);
        hi.Insert(12);
        hi.Insert(13);
        hi.Insert(0);
        hi.Insert(1);
        hi.Insert(1);
        hi.Insert(0);
        hi.Insert(55);
        hi.Insert(40);
        hi.Insert(23);

        size_t size = hi.Size();

        for (size_t i = 0; i < size; i++)
        {
            PRINTLN(hi.Peek());
            hi.Remove();
        }

        PRINTLN("***********************");

        Heap<std::string> hs;

        hs.Insert("hello");
        hs.Insert("world");
        hs.Insert("once");
        hs.Insert("upon");
        hs.Insert("a");
        hs.Insert("time");

        size = hs.Size();

        for (size_t i = 0; i < size; i++)
        {
            PRINTLN(hs.Peek());
            hs.Remove();
        }

        return 0;
    }

#else  // heap class implementation uses extra type parameter for comparator

    #include "heap_vector_param.hpp"

    bool CompareString(const std::string &a, const std::string &b)
    {
        return a < b;
    }

    int main(int argc, char **argv)
    {
        Heap<int, decltype(&Less<int>)> hi(Less<int>);

        hi.Insert(10);
        hi.Insert(12);
        hi.Insert(13);
        hi.Insert(0);
        hi.Insert(1);
        hi.Insert(1);
        hi.Insert(0);
        hi.Insert(55);
        hi.Insert(40);
        hi.Insert(23);

        size_t size = hi.Size();

        for (size_t i = 0; i < size; i++)
        {
            PRINTLN(hi.Peek());
            hi.Remove();
        }

        PRINTLN("***********************");

        Heap<std::string, decltype(&CompareString)> hs(CompareString);

        hs.Insert("hello");
        hs.Insert("world");
        hs.Insert("once");
        hs.Insert("upon");
        hs.Insert("a");
        hs.Insert("time");

        size = hs.Size();

        for (size_t i = 0; i < size; i++)
        {
            PRINTLN(hs.Peek());
            hs.Remove();
        }

        return 0;
    }

#endif  // TYPE_ERASURE