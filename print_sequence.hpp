#ifndef PRINT_SEQUENCE_H
#define PRINT_SEQUENCE_H

#include <iostream>

#define PRINT(s)     std::cout << (s)
#define PRINTLN(s)   PRINT(s) << std::endl

template <typename T, template <typename> class S>
void PrintSequence(const S<T> &sequence)
{
    for (int i = 0; i < sequence.Size(); i++)
    {
        PRINT(*sequence.AtIndex(i));
        PRINT(" --- ");
    }
    PRINTLN("");
}

template <typename T, template <typename> class S>
void ITPrintSequence(const S<T> &sequence)
{
    typename S<T>::ConstIterator it = sequence.Begin();

    while (it != sequence.End())
    {
        PRINT(*it++);
        PRINT(" ");
    }
    PRINTLN("");
}

#endif  // PRINT_SEQUENCE_H