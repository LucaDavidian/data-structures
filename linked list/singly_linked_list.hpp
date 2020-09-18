#ifndef SINGLY_LINKED_LIST_H
#define SINGLY_LINKED_LIST_H

#include <cstddef>  
#include <utility>
#include <exception>

using std::size_t;

class ListEmptyException : public std::exception {};

template <typename T>
class SinglyLinkedList
{
private:
    struct Node
    {
        T data;
        Node *next;
    };
public:
    class Iterator  // forward iterator
    {
    friend class SinglyLinkedList<T>;  // corresponding instantiation of class template SinglyLinkedList is friend
    public:
        Iterator &operator++() { mPrevious = mCurrent; mCurrent = mCurrent->next; return *this; }
        Iterator operator++(int) { Iterator temp = *this; ++*this; return temp; }
        T &operator*() { return mCurrent->data; }
        T *operator->() { return &mCurrent->data; }
        bool operator==(const Iterator &other) { return mCurrent == other.mCurrent && mPrevious == other.mPrevious; }
        bool operator!=(const Iterator &other) { return !(*this == other); }
    private:
        Iterator(Node *current, Node *previous) : mCurrent(current), mPrevious(previous) {}   // private constructor (only LinkedList can construct an Iterator)
        
        Node *mCurrent;
        Node *mPrevious;
    };
    class ConstIterator  
    {
    friend class SinglyLinkedList;   // corresponding instantiation of class template SinglyLinkedList is friend
    public:
        ConstIterator(const Iterator &iterator) : mCurrent(iterator.mCurrent), mPrevious(iterator.mPrevious) {} // implicit conversion from Iterator to ConstIterator
        
        const T &operator*() { return mCurrent->data; }
        const T *operator->() { return &mCurrent->data; }
        ConstIterator &operator++() { mPrevious = mCurrent; mCurrent = mCurrent->next; return *this; }
        ConstIterator operator++(int) { ConstIterator temp(*this); ++*this; return temp; }
        bool operator==(const ConstIterator &other) const { return mCurrent == other.mCurrent && mPrevious == other.mPrevious; }
        bool operator!=(const ConstIterator &other) const { return !(this->operator==(other)); }
    private:
        ConstIterator(Node *current, Node *previous) : mCurrent(current), mPrevious(previous) {}  // private constructor (only LinkedList can construct an Iterator)
        
        Node *mCurrent;
        Node *mPrevious;
    };
public:
    SinglyLinkedList() : mFirst(nullptr), mNumElements(0) {}

    ~SinglyLinkedList() { Clear(); }

    /**** checking if empty and getting size are O(1) ****/
    bool Empty() const { return !mFirst; }
    size_t Size() const { return mNumElements; }

    void Clear() { while (!Empty()) RemoveFirst(); }

    Iterator Begin() { return Iterator(mFirst, nullptr); }
    ConstIterator Begin() const { return ConstIterator(mFirst, nullptr); }
    ConstIterator CBegin() const { return ConstIterator(mFirst, nullptr); }
    Iterator End() { Node *temp = mFirst; if (!Empty()) while (temp->next) temp = temp->next; return Iterator(nullptr, temp); }
    ConstIterator End() const { Node *temp = mFirst; if (!Empty()) while (temp->next) temp = temp->next; return Iterator(nullptr, temp); }
    ConstIterator CEnd() const { Node *temp = mFirst; if (!Empty()) while (temp->next) temp = temp->next; return Iterator(nullptr, temp); }

    /**** inserting first element is O(1) ****/
    template <typename U>
    void InsertFirst(U &&element);
    /**** inserting last element is O(n) ****/
    template <typename U>
    void InsertLast(U &&element);
    /**** inserting an element at arbitrary position is O(1) ****/
    template <typename U>
    Iterator Insert(const Iterator &iterator, U &&element);

    /**** removing first element is O(1) ****/
    void RemoveFirst();
    /**** removinglast element is O(n) ****/
    void RemoveLast();
    /**** removing an element at arbitrary position is O(1) ****/
    Iterator Remove(const Iterator &iterator);

    /**** accessing first element is O(1) ****/
    T &First() { return const_cast<T&>(static_cast<const SinglyLinkedList&>(*this).First()); }
    const T &First() const { if (Empty()) throw ListEmptyException(); return mFirst->data; }
    /**** accessing last element is O(n) ****/
    T &Last(){ return const_cast<T&>(static_cast<const SinglyLinkedList&>(*this).Last()); }
    const T&Last() const { if (Empty()) throw ListEmptyException(); Node *current = mFirst; while (current->next) current = current->next; return current->data; }

    /**** searching for an element is O(n) ****/
    Iterator Find(const T &key) const;

    /**** index/iterator conversion operations are O(n) ****/
    Iterator AtIndex(size_t index) const { Node *current = mFirst; while ((signed)index--) current = current->next; return Iterator(current, current ? current->previous : nullptr); }
    size_t IndexOf(const Iterator &iterator) const { Iterator it = Begin(); size_t count = 0; while (it != iterator) { count++; ++it;} return count; }
private:
    Node *mFirst;
    size_t mNumElements;
};

// begin and end functions (to use in range-for loop)
template <typename T>
typename SinglyLinkedList<T>::Iterator begin(SinglyLinkedList<T> &list)
{
    return list.Begin();
}

template <typename T>
typename SinglyLinkedList<T>::Iterator end(SinglyLinkedList<T> &list)
{
    return list.End();
}

template <typename T>
typename SinglyLinkedList<T>::ConstIterator begin(const SinglyLinkedList<T> &list)
{
    return list.Begin();
}

template <typename T>
typename SinglyLinkedList<T>::ConstIterator end(const SinglyLinkedList<T> &list)
{
    return list.End();
}

template <typename T>
template <typename U>
void SinglyLinkedList<T>::InsertFirst(U &&element)
{
    Node *newNode = new Node{ std::forward<U>(element), nullptr };

    newNode->next = mFirst;
    mFirst = newNode;

    mNumElements++;
}

template <typename T>
template <typename U>
void SinglyLinkedList<T>::InsertLast(U &&element)
{
    if (Empty())
        InsertFirst(std::forward<U>(element));
    else
    {
        Node *newNode = new Node{ std::forward<U>(element), nullptr };

        Node *current = mFirst;

        while (current->next)
            current = current->next;

        current->next = newNode;
    }

    mNumElements++;
}

template <typename T>
template <typename U>
typename SinglyLinkedList<T>::Iterator SinglyLinkedList<T>::Insert(const Iterator &position, U &&element)
{
    Node *newNode = new Node{ std::forward<U>(element), nullptr };

    if (position.mCurrent && !position.mPrevious)  // insert first, non empty list
    {
        newNode->next = mFirst;
        mFirst = newNode;
    }
    else if (!position.mCurrent && position.mPrevious)  // insert last, non empty list
    {
        Node *current = mFirst;

        while (current->next)
            current = current->next;

        current->next = newNode;
    }
    else if (!position.mCurrent && !position.mPrevious)  // insert first, empty list
        mFirst = newNode;
    else  // insert in middle of non empty list
    {   
        position.mPrevious->next = newNode;
        newNode->next = position.mCurrent; 
    }

    mNumElements++;

    return Iterator(newNode, position.mPrevious); 
}

template <typename T>
void SinglyLinkedList<T>::RemoveFirst()
{
    if (Empty())
        throw std::exception();

    Node *temp = mFirst;
    mFirst = mFirst->next;
    delete temp;

    mNumElements--;
}

template <typename T>
void SinglyLinkedList<T>::RemoveLast()
{
    if (Empty())
        throw std::exception();

    Node *current = mFirst, *previous = nullptr;

    while (current->next)
    {
        previous = current;
        current = current->next;
    }

    if (!previous)
        RemoveFirst();
    else
    {
        previous->next = nullptr;
        delete current;
    }   

    mNumElements--;
}

template <typename T>
typename SinglyLinkedList<T>::Iterator SinglyLinkedList<T>::Remove(const Iterator &position)
{
    if (Empty())
        throw std::exception();

    if (position.mCurrent && !position.mPrevious)  // remove first
        mFirst = mFirst->next;
    else // remove non first
        position.mPrevious->next = position.mCurrent->next;

    Node *newCurrent = position.mCurrent->next;
    delete position.mCurrent;

    mNumElements--;

    return Iterator(newCurrent, position.mPrevious);
}

template <typename T>
typename SinglyLinkedList<T>::Iterator SinglyLinkedList<T>::Find(const T &key) const
{
    Node *current = mFirst, *previous = nullptr;

    while (current && key != current->data)
    {
        previous = current;
        current = current->next;
    }

    return Iterator(current, previous);
}

#endif  // SINGLY_LINKED_LIST_H