/**** ordered map - binary search tree implementation ****/

#ifndef MAP_H
#define MAP_H

#include <cstddef>
#include <utility>
#include "../../../../type erasure/type erasure - function/function.hpp"

using std::size_t;

/**** default comparator function object ****/
template <typename T>
bool Less(const T &a, const T &b)
{
    return a < b;
}

/**** entry (pair) class ****/
template <typename K, typename V>
class Entry
{
public:
    template <typename FWDK, typename FWDV>
    Entry(FWDK &&key, FWDV &&value) : mKey(std::forward<FWDK>(key)), mValue(std::forward<FWDV>(value)) {}

    K &Key() { return mKey; }
    const K &Key() const { return mKey; }
    V &Value() { return mValue; }
    const V &Value() const { return mValue; }
private:
    K mKey;
    V mValue;
};

/**** map class ****/
template <typename K, typename V>
class Map
{
private:
    struct Node
    {
        Entry<const K,V> mData;
        Node *mParent;
        Node *mLeftChild;
        Node *mRightChild;
    };
public:
    class Iterator
    {
    friend class Map;
    public:
        Entry<const K,V> &operator*() const { return mCurrent->mData; }
        Entry<const K,V> *operator->() const { return &mCurrent->mData; }
        Iterator &operator++();
        Iterator operator++(int) { Iterator temp(mCurrent); ++*this; return temp; }
        Iterator &operator--();
        Iterator operator--(int) { Iterator temp(mCurrent); --*this; return temp; }
        bool operator==(const Iterator &other) { return mCurrent == other.mCurrent; }
        bool operator!=(const Iterator &other) { return !(*this == other); }
    private:
        Iterator(Node *current) : mCurrent(current) {}

        Node *mCurrent;
    };
    class ConstIterator
    {
    friend class Map;
    public:
        ConstIterator(const Iterator &other) : mCurrent(other.mCurrent) {}  // public implicit conversion from Iterator
        
        const Entry<const K,V> &operator*() const { return mCurrent->mData; }
        const Entry<const K,V> *operator->() const { return &mCurrent->mData; }
        ConstIterator &operator++();
        ConstIterator operator++(int) { ConstIterator temp(mCurrent); ++*this; return temp; }
        ConstIterator &operator--();
        ConstIterator operator--(int) { ConstIterator temp(mCurrent); --*this; return temp; }
        bool operator==(const ConstIterator &other) { return mCurrent == other.mCurrent; }
        bool operator!=(const ConstIterator &other) { return !(*this == other); }
    private:
        ConstIterator(Node *current) : mCurrent(current) {}

        Node *mCurrent;
    };
public:
    Map(const Function<bool(const K&, const K&)> &comparator = &Less<K>) : mRoot(nullptr), mNumElements(0), mComparator(comparator) {}

    bool Empty() const { return mNumElements == 0; }
    size_t Size() const { return mNumElements; }

    Iterator Begin();
    ConstIterator Begin() const;
    ConstIterator CBegin() const { return Begin(); }
    Iterator End() { return Iterator(nullptr); }
    ConstIterator End() const { return Iterator(nullptr); }
    ConstIterator CEnd() const { return End(); }

    template <typename FWDK, typename FWDV>
    void Insert(FWDK &&key, FWDV &&value);
    void Insert(const Entry<K,V> &entry) { Insert(entry.Key(), entry.Value()); }

    void Remove(const K &key);
    void Remove(const ConstIterator &iterator);

    ConstIterator Find(const K &key) const;
private:
    Node *mRoot;
    size_t mNumElements;

    Function<bool(const K&, const K&)> mComparator;
};

/**** map iterator implementation ****/

template <typename K, typename V>
typename Map<K,V>::Iterator &Map<K,V>::Iterator::operator++()
{
    if (mCurrent->mRightChild)    
    {
        mCurrent = mCurrent->mRightChild;

        while (mCurrent->mLeftChild)
            mCurrent = mCurrent->mLeftChild;
    }
    else 
    {
        Node *temp = nullptr;

        do 
        {
            temp = mCurrent;
            mCurrent = mCurrent->mParent;
        } while (mCurrent && temp == mCurrent->mRightChild);
    }

    return *this;
}

template <typename K, typename V>
typename Map<K,V>::Iterator &Map<K,V>::Iterator::operator--()
{
    if (mCurrent->mLeftChild)
    {
        mCurrent = mCurrent->mLeftChild;

        while (mCurrent->mRightChild)
            mCurrent = mCurrent->mRightChild;
    }
    else
    {
        Node *temp = nullptr;

        do
        {
            temp = mCurrent;
            mCurrent = mCurrent->mParent;
        } while (mCurrent && temp == mCurrent->mLeftChild);
    }

    return *this;    
}

template <typename K, typename V>
typename Map<K,V>::ConstIterator &Map<K,V>::ConstIterator::operator++()
{
    // find inorder successor
    if (mCurrent->mRightChild)    
    {
        mCurrent = mCurrent->mRightChild;
        
        while (mCurrent->mLeftChild)
            mCurrent = mCurrent->mLeftChild;
    }
    else 
    {
        Node *temp = nullptr;

        do 
        {
            temp = mCurrent;
            mCurrent = mCurrent->mParent;
        } while (mCurrent && temp == mCurrent->mRightChild);
    }
        
    return *this;
}

template <typename K, typename V>
typename Map<K,V>::ConstIterator &Map<K,V>::ConstIterator::operator--()
{
    if (mCurrent->mLeftChild)
    {
        mCurrent = mCurrent->mLeftChild;

        while (mCurrent->mRightChild)
            mCurrent = mCurrent->mRightChild;
    }
    else
    {
        Node *temp = nullptr;

        do
        {
            temp = mCurrent;
            mCurrent = mCurrent->mParent;
        } while (mCurrent && temp == mCurrent->mLeftChild);
    }

    return *this; 
}

/**** map implementation ****/

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::Begin()
{
    if (Empty())
        return End();

    // find first node in inorder traversal
    Node *current = mRoot;

    while (current->mLeftChild)
        current = current->mLeftChild;

    return Iterator(current);
}

template <typename K, typename V>
typename Map<K,V>::ConstIterator Map<K,V>::Begin() const
{
    if (Empty())
        return End();

    // find first node in inorder traversal
    Node *current = mRoot;

    while (current->mLeftChild)
        current = current->mLeftChild;

    return ConstIterator(current);
}

template <typename K, typename V>
template <typename FWDK, typename FWDV>
void Map<K,V>::Insert(FWDK &&key, FWDV &&value)
{
    Node *newNode = new Node{ Entry<const K,V>(std::forward<FWDK>(key), std::forward<FWDV>(value)), nullptr, nullptr, nullptr };

    if (Empty())
        mRoot = newNode;
    else
    {
        Node *parent = nullptr, *current = mRoot;
        bool isLeftChild = false;

        while (current)
        {
            parent = current;

            if (mComparator(newNode->mData.Key(), current->mData.Key()))  // new key < current node's key
            {
                current = current->mLeftChild;
                isLeftChild = true;
            }
            else    // new key >= current node's key
            {
                current = current->mRightChild;
                isLeftChild = false;
            }
        }

        if (isLeftChild)   // new node is left child
            parent->mLeftChild = newNode;
        else  // new node is right child
            parent->mRightChild = newNode;

        newNode->mParent = parent;
    }
    
    mNumElements++;
}

template <typename K, typename V>
void Map<K,V>::Remove(const K &key)
{
    if (Empty())
        return;

    ConstIterator it = Find(key);

    if (it != End())
        Remove(it);
}

template <typename K, typename V>
void Map<K,V>::Remove(const ConstIterator &iterator)
{
    Node *current = iterator.mCurrent;

    if (!current->mLeftChild && !current->mRightChild)
    {
        if (current == mRoot)
            mRoot = nullptr;
        else if (current == current->mParent->mLeftChild)
            current->mParent->mLeftChild = nullptr;
        else // current == current->mParent->mRightChild
            current->mParent->mRightChild = nullptr;
    }
    else if (!current->mLeftChild)
    {
        if (current == mRoot)
        {
            mRoot = current->mRightChild;
            current->mRightChild->mParent = nullptr;
        }
        else if (current == current->mParent->mLeftChild)
        {
            current->mParent->mLeftChild = current->mRightChild;
            current->mRightChild->mParent = current->mParent;
        }
        else // current == current->mParent->mRightChild
        {
            current->mParent->mRightChild = current->mRightChild;
            current->mRightChild->mParent = current->mParent;
        }
    }
    else if (!current->mRightChild)
    {
        if (current == mRoot)
        {
            mRoot = current->mLeftChild;
            current->mLeftChild->mParent = nullptr;
        }
        else if (current == current->mParent->mLeftChild)
        {
            current->mParent->mLeftChild = current->mLeftChild;
            current->mLeftChild->mParent = current->mParent;
        }   
        else // current == current->mParent->mRightChild
        {
            current->mParent->mRightChild = current->mLeftChild;
            current->mLeftChild->mParent = current->mParent;
        }
    }
    else
    {
        Node *inOrderNext = current->mRightChild;
        
        while (inOrderNext->mLeftChild)
            inOrderNext = inOrderNext->mLeftChild;

        if (inOrderNext == current->mRightChild)
        {
            inOrderNext->mLeftChild = current->mLeftChild;
            current->mLeftChild->mParent = inOrderNext;

            if (current == mRoot)
                mRoot = inOrderNext;
            else if (current == current->mParent->mLeftChild)
                current->mParent->mLeftChild = inOrderNext;
            else // current == current->mParent->mRightChild
                current->mParent->mRightChild = inOrderNext;

            inOrderNext->mParent = current->mParent;
        }
        else
        {
            inOrderNext->mParent->mLeftChild = inOrderNext->mRightChild;

            if (inOrderNext->mRightChild)
                inOrderNext->mRightChild->mParent = inOrderNext->mParent;

            inOrderNext->mLeftChild = current->mLeftChild;
            current->mLeftChild->mParent = inOrderNext;
            inOrderNext->mRightChild = current->mRightChild;
            current->mRightChild->mParent = inOrderNext;

            if (current == mRoot)
                mRoot = inOrderNext;
            else if (current == current->mParent->mLeftChild)
                current->mParent->mLeftChild = inOrderNext;
            else // current == current->mParent->mRightChild
                current->mParent->mRightChild = inOrderNext;

            inOrderNext->mParent = current->mParent;
        }
    }

    delete current;

    mNumElements--;
}

template <typename K, typename V>
typename Map<K,V>::ConstIterator Map<K,V>::Find(const K &key) const
{
    Node *current = mRoot;

    while (current)
    {
        if (mComparator(key, current->mData.Key()))
            current = current->mLeftChild;
        else if (mComparator(current->mData.Key(), key))
            current = current->mRightChild;
        else 
            return ConstIterator(current);
    }

    return End();
}

#endif  // MAP_H
