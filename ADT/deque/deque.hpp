#ifndef DEQUE_H
#define DEQUE_H

#include "../../linked list/double_ended_doubly_linked_list.hpp"

template <typename T, template <typename> class DS = DoublyLinkedList>
class Deque
{
public:
    Deque() = default;
    ~Deque() = default;

    bool Empty() const { return mDataStruct.Empty(); }
    size_t Size()const { return mDataStruct.Size(); }
    
    template <typename U>
    void InsertFront(U &&element) { mDataStruct.InsertFirst(std::forward<U>(element)); }
    template <typename U>
    void InsertBack(U &&element) { mDataStruct.InsertLast(std::forward<U>(element)); }

    void RemoveFront() { mDataStruct.RemoveFirst(); }
    void RemoveBack() { mDataStruct.RemoveLast(); } 

    T &Front() { return const_cast<T&>(static_cast<const Deque*>(this)->Front()); }
    const T &Front() const { return mDataStruct.First(); }
    T &Back() { return const_cast<T&>(static_cast<const Deque*>(this)->Back()); }
    const T &Back() const { return mDataStruct.Last(); }
private:
    DS<T> mDataStruct;
};

#endif  // DEQUE_H