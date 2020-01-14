#ifndef SEQUENCE_REVERSE_H
#define SEQUENCE_REVERSE_H

#include "ADT/stack/stack.hpp"

template <typename T, template <class> class S>
void SequenceReverse(S<T> &sequence)
{
    Stack<T> stack;

    while (!sequence.Empty())
    {
        stack.Push(sequence.First());
        sequence.RemoveFirst();
    }

    while (!stack.Empty())
    {
        sequence.InsertLast(stack.Top());
        stack.Pop();
    }
}

#endif  // SEQUENCE_REVERSE_H