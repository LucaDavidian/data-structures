#ifndef DOUBLE_ENDED_SINGLY_LINKED_LIST_H
#define DOUBLE_ENDED_SINGLY_LINKED_LIST_H

#include <cstddef>
#include <utility>
#include <exception>

using std::size_t;

class ListEmptyException : public std::exception {};

template <typename T>
class DE_SinglyLinkedList
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
    friend class DE_SinglyLinkedList;  // corresponding instantiation of class template DESinglyLinkedList is friend
    public:
        Iterator &operator++() { mPrevious = mCurrent; mCurrent = mCurrent->next; return *this; }
        Iterator operator++(int) { Iterator temp = *this; ++*this; return temp; }
        T &operator*() { return mCurrent->data; }
        T *operator->() { return &mCurrent->data; }
        bool operator==(const Iterator &other) { return mCurrent == other.mCurrent && mPrevious == other.mPrevious; }
        bool operator!=(const Iterator &other) { return !(*this == other); }
    private:
        Iterator(Node *current, Node *previous) : mCurrent(current), mPrevious(previous) {}  // private constructor (only LinkedList can construct an Iterator)
        
        Node *mCurrent;
        Node *mPrevious;
    };
    class ConstIterator  
    {
    friend class DE_SinglyLinkedList;   // corresponding instantiation of class template DESinglyLinkedList is friend
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
    DE_SinglyLinkedList() : mFirst(nullptr), mLast(nullptr), mNumElements(0) {}

    ~DE_SinglyLinkedList() { Clear(); }

    /**** checking if empty or getting size is O(1) ****/
    bool Empty() const { return !mFirst; }
    size_t Size() const { return mNumElements; }

    void Clear() { while (!Empty()) RemoveFirst(); }

    Iterator Begin() { return Iterator(mFirst, nullptr); }
    ConstIterator Begin() const { return ConstIterator(mFirst, nullptr); }
    ConstIterator CBegin() const { return ConstIterator(mFirst, nullptr); }
    Iterator End() { return Iterator(nullptr, mLast); }
    ConstIterator End() const { return ConstIterator(nullptr, mLast); }
    ConstIterator CEnd() const { return ConstIterator(nullptr, mLast); }

    /**** inserting first or last element is O(1) ****/
    template <typename U>
    void InsertFirst(U &&element);
    template <typename U>
    void InsertLast(U &&element);
    /**** inserting an element at arbitrary position is O(1) ****/
    template <typename U>
    Iterator Insert(const Iterator &iterator, U &&element);

    /**** removing first element is O(1) ****/
    void RemoveFirst();
    /**** removing last element is O(n) ****/
    void RemoveLast();
    /**** removing an element at arbitrary position is O(1) ****/
    Iterator Remove(const Iterator &iterator);
    
    /**** accessing first and last element is O(1) ****/
    T &First() { return const_cast<T&>(static_cast<const DE_SinglyLinkedList&>(*this).First()); }
    const T &First() const { if (Empty()) throw std::exception(); return mFirst->data; }
    T &Last(){ return const_cast<T&>(static_cast<const DE_SinglyLinkedList&>(*this).Last()); }
    const T&Last() const{ if (Empty()) throw std::exception(); return mLast->data; }

    /**** searching for an element is O(n) ****/
    Iterator Find(const T&) const;

    /**** index/iterator conversion operations are O(n) ****/
    Iterator AtIndex(size_t index) const { Node *current = mFirst; while ((signed)index--) current = current->next; return Iterator(current, current ? current->previous : nullptr); }
    size_t IndexOf(const Iterator &iterator) const { Iterator it = Begin(); size_t count = 0; while (it != iterator) { count++; ++it;} return count; }
private:
    Node *mFirst;
    Node *mLast;
    size_t mNumElements;
};

// begin and end functions (to use in range-for loop)
template <typename T>
typename DE_SinglyLinkedList<T>::Iterator begin(DE_SinglyLinkedList<T> &list)
{
    return list.Begin();
}

template <typename T>
typename DE_SinglyLinkedList<T>::Iterator end(DE_SinglyLinkedList<T> &list)
{
    return list.End();
}

template <typename T>
typename DE_SinglyLinkedList<T>::ConstIterator begin(const DE_SinglyLinkedList<T> &list)
{
    return list.Begin();
}

template <typename T>
typename DE_SinglyLinkedList<T>::ConstIterator end(const DE_SinglyLinkedList<T> &list)
{
    return list.End();
}

template <typename T>
template <typename U>
void DE_SinglyLinkedList<T>::InsertFirst(U &&element)
{
    Node *newNode = new Node{ std::forward<U>(element), nullptr };

    if (Empty())
        mLast = newNode;
    else
        newNode->next = mFirst;
    
    mFirst = newNode;

    mNumElements++;
}

template <typename T>
template <typename U>
void DE_SinglyLinkedList<T>::InsertLast(U &&element)
{
    Node *newNode = new Node{ std::forward<U>(element), nullptr };

    if (Empty())
        mFirst = newNode;
    else
        mLast->next = newNode;
    
    mLast = newNode;

    mNumElements++;
}

template <typename T>
template <typename U>
typename DE_SinglyLinkedList<T>::Iterator DE_SinglyLinkedList<T>::Insert(const Iterator &position, U &&element)
{
    Node *newNode = new Node{ std::forward<U>(element), nullptr };

    if (position.mCurrent && !position.mPrevious)  // insert first, non empty list
    {
        newNode->next = mFirst;
        mFirst = newNode;
    }
    else if (!position.mCurrent && position.mPrevious)  // insert last, non empty list
    {
        mLast->next = newNode;
        mLast = newNode;
    }
    else if (!position.mCurrent && !position.mPrevious)  // insert first, empty list
    {
        mFirst = newNode;
        mLast = newNode;
    }
    else  // insert in middle of non empty list
    {
        position.mPrevious->next = newNode;
        newNode->next = position.mCurrent;
    }

    mNumElements++;

    return Iterator(newNode, position.mPrevious);
}

template <typename T>
void DE_SinglyLinkedList<T>::RemoveFirst()
{
    if (Empty())
        throw std::exception();

    Node *temp = mFirst;
    mFirst = mFirst->next;
    delete temp;

    if (Empty())
        mLast = nullptr;

    mNumElements--;
}

template <typename T>
void DE_SinglyLinkedList<T>::RemoveLast()
{
    if (Empty())
        throw std::exception();

    Node *current = mFirst, *previous = nullptr;

    // while (current->next != mLast)
    //     current = current->next;
    
    // if (current == mFirst)
    //     RemoveFirst();
    // else
    // {
    //     current->next = nullptr;
    //     delete mLast;
    //     mLast = current;
    // }

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
        mLast = previous;
        delete current;
    }   

    mNumElements--;
}

template <typename T>
typename DE_SinglyLinkedList<T>::Iterator DE_SinglyLinkedList<T>::Remove(const Iterator &position)
{
    if (Empty())
        throw std::exception();

    if (position.mCurrent && !position.mPrevious)   // remove first
    {
        mFirst = mFirst->next;

        if (Empty())
            mLast = nullptr;
    }
    else   // remove non first
    {
        position.mPrevious->next = position.mCurrent->next;

        if (!position.mCurrent->next)  // deleted last element
            mLast = position.mPrevious;
    }

    Node *newCurrent = position.mCurrent->next;  // if first is deleted position.mPrevious is null
    delete position.mCurrent;
    mNumElements--;

    return Iterator(newCurrent, position.mPrevious); 
}

template <typename T>
typename DE_SinglyLinkedList<T>::Iterator DE_SinglyLinkedList<T>::Find(const T &key) const
{
    Node *current = mFirst, *previous = nullptr;

    while (current && key != current->data)
    {
        previous = current;
        current = current->next;
    }

    return Iterator(current, previous);
}

#endif  // DOUBLE_ENDED_SINGLY_LINKED_LIST_H
