#ifndef DOUBLE_ENDED_DOUBLY_LINKED_LIST
#define DOUBLE_ENDED_DOUBLY_LINKED_LIST

#include <cstddef>
#include <utility>
#include <exception>

using std::size_t;

class ListEmptyException : public std::exception {};

template <typename T>
class DoublyLinkedList;

template <typename T>
void swap(DoublyLinkedList<T> &a, DoublyLinkedList<T> &b)
{
    a.Swap(b);
}

template <typename T>
class DoublyLinkedList
{
private:
    struct Node
    {
        T data;
        Node *next;
        Node *previous;
    };
public:
    class Iterator  // bidirectional iterator (doubly linked list)
    {
    friend class DoublyLinkedList;   // corresponding instantiation of class template DoublyLinkedList is friend
    public:
        T &operator*() { return mCurrent->data; }
        T *operator->() { return &mCurrent->data; }
        Iterator &operator++() { mPrevious = mCurrent; mCurrent = mCurrent->next; return *this; }
        Iterator operator++(int) { Iterator temp(*this); ++*this; return temp; }
        Iterator &operator--() { mCurrent = mPrevious; mPrevious = mPrevious->previous; return *this; }      
        Iterator operator--(int) { Iterator temp = *this; this->operator--(); return temp;}
        bool operator==(const Iterator &other) const { return mCurrent == other.mCurrent && mPrevious == other.mPrevious; }
        bool operator!=(const Iterator &other) const { return !(this->operator==(other)); }
        // operator class ConstIterator() { return ConstIterator(mCurrent, mPrevious); }  // implicit conversion from Iterator to ConstIterator
    private:
        Iterator(Node *current, Node *previous) : mCurrent(current), mPrevious(previous) {}  // private constructor (only LinkedList can construct an Iterator)
        
        Node *mCurrent;
        Node *mPrevious;
    };
    class ConstIterator  
    {
    friend class DoublyLinkedList;   // corresponding instantiation of class template DoublyLinkedList is friend
    public:
        const T &operator*() { return mCurrent->data; }
        const T *operator->() { return &mCurrent->data; }
        ConstIterator &operator++() { mPrevious = mCurrent; mCurrent = mCurrent->next; return *this; }
        ConstIterator operator++(int) { ConstIterator temp(*this); ++*this; return temp; }
        ConstIterator &operator--() { mCurrent = mPrevious; mPrevious = mPrevious->previous; return *this; }      
        ConstIterator operator--(int) { ConstIterator temp = *this; this->operator--(); return temp;}
        bool operator==(const ConstIterator &other) const { return mCurrent == other.mCurrent && mPrevious == other.mPrevious; }
        bool operator!=(const ConstIterator &other) const { return !(this->operator==(other)); }
    private:
        ConstIterator(Node *current, Node *previous) : mCurrent(current), mPrevious(previous) {}  // private constructor (only LinkedList can construct an Iterator)
        ConstIterator(const Iterator &iterator) : mCurrent(iterator.mCurrent), mPrevious(iterator.mPrevious) {} // implicit conversion from Iterator to ConstIterator
        
        Node *mCurrent;
        Node *mPrevious;
    };
public:
    DoublyLinkedList() : mFirst(nullptr), mLast(nullptr), mNumElements(0) {}
    DoublyLinkedList(const DoublyLinkedList &other); 
    DoublyLinkedList(DoublyLinkedList &&other); 

    ~DoublyLinkedList() { Clear(); }

    DoublyLinkedList &operator=(const DoublyLinkedList &other); 
    DoublyLinkedList &operator=(DoublyLinkedList &&other); 

    void Swap(DoublyLinkedList &other);

    /**** checking if empty and getting element count are O(1) ****/
    bool Empty() const { return !mFirst; }
    size_t Size() const { return mNumElements; }

    void Clear() { while (!Empty()) RemoveFirst(); }

    Iterator Begin() { return Iterator(mFirst, nullptr); }
    ConstIterator Begin() const { return ConstIterator(mFirst, nullptr); }
    ConstIterator CBegin() const { return ConstIterator(mFirst, nullptr); }
    Iterator End() { return Iterator(nullptr, mLast); }
    ConstIterator End() const { return ConstIterator(nullptr, mLast); }
    ConstIterator CEnd() const { return ConstIterator(nullptr, mLast); }

    /**** inserting first element and last element are O(1) ****/
    template <typename U>
    void InsertFirst(U &&element);
    template <typename U>
    void InsertLast(U &&element);
    /**** inserting with iterator is O(1) ****/
    template <typename U>
    Iterator Insert(const Iterator&, U &&element);
    
    /*** removing first and last element are O(1) ****/
    void RemoveFirst();
    void RemoveLast();
    /**** removing with iterator is O(1) ****/
    Iterator Remove(const Iterator &iterator);

    /**** accessing first and last element is O(1) ****/
    T &First() { return const_cast<T&>(static_cast<const DoublyLinkedList&>(*this).First()); }
    const T&First() const { if (Empty()) throw ListEmptyException(); return mFirst->data; }
    T &Last() { return const_cast<T&>(static_cast<const DoublyLinkedList&>(*this).Last()); }
    const T &Last() const { if (Empty()) throw ListEmptyException(); return mLast->data; }

    /**** searching for an element is O(n) ****/
    Iterator Find(const T &key);

    /**** index/iterator conversion operations are O(n) ****/
    Iterator AtIndex(size_t index) const { Node *current = mFirst; while ((signed)index--) current = current->next; return Iterator(current, current ? current->previous : nullptr); }
    size_t IndexOf(const Iterator &iterator) const { Iterator it = Begin(); size_t count = 0; while (it != iterator) { count++; ++it;} return count; }
private:
    Node *mFirst;
    Node *mLast;
    size_t mNumElements;
};

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(const DoublyLinkedList &other) : mFirst(nullptr), mLast(nullptr), mNumElements(0)
{
    DoublyLinkedList::ConstIterator it = other.Begin();

    while (it != other.End())
        InsertLast(*it++);
}
    
template <typename T>    
DoublyLinkedList<T>::DoublyLinkedList(DoublyLinkedList &&other) : mFirst(other.mFirst), mLast(other.mLast), mNumElements(other.mNumElements)
{
    other.mFirst = other.mLast = nullptr;
    other.mNumElements = 0;
}

template <typename T>
DoublyLinkedList<T> &DoublyLinkedList<T>::operator=(const DoublyLinkedList &other)
{
    DoublyLinkedList temp = other;
    Swap(temp);
    
    return *this;
}

template <typename T>
DoublyLinkedList<T> &DoublyLinkedList<T>::operator=(DoublyLinkedList &&other)
{
    Swap(other);

    return *this;
}

template <typename T>
void DoublyLinkedList<T>::Swap(DoublyLinkedList &other)
{
    // swap pointers
    Node *tempFirst = mFirst;
    Node *tempLast = mLast;
    mFirst = other.mFirst;
    mLast = other.mLast;
    other.mFirst = tempFirst;
    other.mLast = tempLast;

    // swap element count
    size_t tempNum = mNumElements;
    mNumElements = other.mNumElements;
    other.mNumElements = tempNum;
}

template <typename T>
template <typename U>
void DoublyLinkedList<T>::InsertFirst(U &&element)
{
    Node *newNode = new Node{ std::forward<U>(element), nullptr, nullptr };

    if (Empty())
        mLast = newNode;
    else
    {
        newNode->next = mFirst;
        mFirst->previous = newNode;
    }

    mFirst = newNode;
    
    mNumElements++;
}

template <typename T>
template <typename U>
void DoublyLinkedList<T>::InsertLast(U &&element)
{
    Node *newNode = new Node{ std::forward<U>(element), nullptr, nullptr };

    if (Empty())
        mFirst = newNode;
    else
    {
        mLast->next = newNode;
        newNode->previous = mLast;
    }

    mLast = newNode;

    mNumElements++;
}


template <typename T>
template <typename U>
typename DoublyLinkedList<T>::Iterator DoublyLinkedList<T>::Insert(const Iterator &position, U &&element)
{
    Node *newNode = new Node{ std::forward<U>(element), nullptr, nullptr };

    if (Empty())  // empty list
    {
        mFirst = newNode;
        mLast = newNode;
    }
    else if (position == Begin())  // insert first, non empty list
    {
        newNode->next = position.mCurrent;
        position.mCurrent->previous = newNode;
        mFirst = newNode;
    }
    else if (position == End())  // insert last, non empty list
    {
        position.mPrevious->next = newNode;
        newNode->previous = position.mPrevious;
        mLast = newNode;
    }
    else  // insert in middle of non empty list
    {
        newNode->next = position.mCurrent;
        newNode->previous = position.mPrevious;
        position.mPrevious->next = newNode;
        position.mCurrent->previous = newNode;
    }

    mNumElements++;
    
    return Iterator(newNode, newNode->previous);
}

template <typename T>
void DoublyLinkedList<T>::RemoveFirst()
{
    if (Empty())
        throw ListEmptyException();

    Node *temp = mFirst;
    mFirst = mFirst->next;

    if (Empty())
        mLast = nullptr;
    else
        mFirst->previous = nullptr;

    delete temp;
    mNumElements--;    
}

template <typename T>
void DoublyLinkedList<T>::RemoveLast()
{
    if (Empty())
        throw ListEmptyException();

    Node *temp = mLast;
    mLast = mLast->previous;

    if (!mLast)
        mFirst = nullptr;
    else
        mLast->next = nullptr;

    delete temp;
    mNumElements--;    
}

template <typename T>
typename DoublyLinkedList<T>::Iterator DoublyLinkedList<T>::Remove(const Iterator &position)
{
    if (Empty())
        throw ListEmptyException();

    if (position == End())
        return End();

    if (position == Begin())   // remove first
    {
        mFirst = mFirst->next;

        if (Empty())
            mLast = nullptr;
        else
            mFirst->previous = nullptr;       
    }
    else     // remove non first
    {
        position.mPrevious->next = position.mCurrent->next;

        if (!position.mCurrent->next)    // deleted last element
            mLast = position.mPrevious;
        else
            position.mCurrent->next->previous = position.mPrevious; 
    }

    Node *newCurrent = position.mCurrent->next;

    delete position.mCurrent;
    mNumElements--;

    return Iterator(newCurrent, position.mPrevious);
}

template <typename T>
typename DoublyLinkedList<T>::Iterator DoublyLinkedList<T>::Find(const T &key) 
{
    // Node *current = mFirst, *previous = nullptr;

    // while (current && key != current->data)
    // {
    //     previous = current;
    //     current = current->next;
    // }

    // return Iterator(current, previous);

    // iterator implementation
    Iterator it = Begin();

    while (it != End())
    {
        if (*it == key)
            return it;   // found
        
        ++it;
    }

    return End();      // not found: return off-the-end iterator
}

#endif  // DOUBLE_ENDED_DOUBLY_LINKED_LIST