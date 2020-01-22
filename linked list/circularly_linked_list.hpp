#ifndef CIRCULARLY_LINKED_LIST_H
#define CIRCULARLY_LINKED_LIST_H

#include <cstddef>
#include <utility>
#include <exception>

using std::size_t;

template <typename T>
class CircularlyLinkedList
{
private:
    struct Node
    {
        T data;
        Node *next;
    };
public:
    CircularlyLinkedList() : mCursor(0), mNumElements(0) {}
    ~CircularlyLinkedList() { while (!Empty()) Remove(); }

    bool Empty() const { return !mCursor; }
    size_t Size() const { return mNumElements; }

    template <typename U>
    void Insert(U&&);

    void Remove();

    void Advance() { if (Empty()) return; mCursor = mCursor->next;}

    T &Front() { return const_cast<T&>(static_cast<const CircularlyLinkedList*>(this)->Front()); }
    const T &Front() const { if (Empty()) throw std::exception(); return mCursor->next->data; }
    T &Back() { return const_cast<T&>(static_cast<const CircularlyLinkedList*>(this)->Back()); }
    const T &Back() const { if (Empty()) throw std::exception(); return mCursor->data; }
private:
    Node *mCursor;
    size_t mNumElements;
};

template <typename T>
template <typename U>
void CircularlyLinkedList<T>::Insert(U &&element)
{
    Node *newNode = new Node{ std::forward<U>(element), nullptr };

    if (Empty())
    {
        mCursor = newNode;
        newNode->next = newNode;
    }
    else
    {
        newNode->next = mCursor->next;
        mCursor->next = newNode;
    }

    mNumElements++;
}

template <typename T>
void CircularlyLinkedList<T>::Remove()
{
    if (Empty())
        throw std::exception();

    Node *temp = mCursor->next;

    if (mCursor == mCursor->next)
        mCursor = nullptr;
    else
        mCursor->next = mCursor->next->next;
    
    delete temp;

    mNumElements--;
}

#endif  // CIRCULARLY_LINKED_LIST_H