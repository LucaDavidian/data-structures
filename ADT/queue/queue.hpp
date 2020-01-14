#ifndef QUEUE_H
#define QUEUE_H

#include <cstddef>

#include "../../list/double_ended_singly_linked_list.hpp"
#include "../../list/circularly_linked_list.hpp"

using std::size_t;

// with double ended linked list all operations are implemented in O(1) constant time
template <typename T, template <typename> class S = DE_SinglyLinkedList>  
class Queue
{
public:
    Queue() = default;
    ~Queue() = default;

    bool Empty() const { return mContainer.Empty(); }
    size_t Size() const { return mContainer.Size(); }

    template <typename U>
    void Enqueue(U &&element) { mContainer.InsertLast(std::forward<U>(element)); }
    void Dequeue() { mContainer.RemoveFirst(); }

    T &Front() { return const_cast<T&>(static_cast<const Queue*>(this)->Front()); }
    const T &Front() const { return mContainer.First(); }
    T &Back() { return const_cast<T&>(static_cast<const Queue*>(this)->Back()); }
    const T &Back() const { return mContainer.Last(); }
private:
    S<T> mContainer;
};

/**** partial class template specialization - circularly linked list implementation ****/

// with circularly linked list all operations are implemented in O(1) constant time
template <typename T>
class Queue<T,CircularlyLinkedList>
{
public:
    Queue() = default;
    ~Queue() = default;

    bool Empty() const { return mList.Empty(); }
    size_t Size() const { return mList.Size(); }

    template <typename U>
    void Enqueue(U &&element) { mList.Insert(std::forward<U>(element)); mList.Advance(); }
    void Dequeue() { mList.Remove(); }

    T &Back() { return const_cast<T&>(static_cast<const Queue*>(this)->Back()); }
    const T &Back() const { return mList.Back(); }
    T &Front() { return const_cast<T&>(static_cast<const Queue*>(this)->Front()); }
    const T &Front() const { return mList.Front(); }

private:
    CircularlyLinkedList<T> mList;
};

#endif  // QUEUE_H