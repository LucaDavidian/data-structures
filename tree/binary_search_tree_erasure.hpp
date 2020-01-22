#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <cstddef>
#include <utility>
#include <exception>

#include "../../type erasure/type erasure - function/function.hpp"

using std::size_t;

template <typename T>
bool Less(const T &a, const T &b)
{
    return a < b;
}

template <typename T>
class BinarySearchTree
{
private:
    struct Node
    {
        T mData;
        Node *mParent;
        Node *mLeftChild;
        Node *mRightChild;
    };
public:
    class Iterator  // forward iterator (inorder traversal)
    {
    friend class BinarySearchTree<T>;
    public:
        T &operator*() const { return mCurrent->mData; }
        T *operator->() const { return &mCurrent->mData; }
        Iterator &operator++();   
        Iterator operator++(int) { Iterator temp(*this); ++*this; return temp; }
        Iterator &operator--();
        Iterator operator--(int) { Iterator temp(mCurrent); --*this; return temp; }
        bool operator==(const Iterator &other) const { return mCurrent == other.mCurrent; }
        bool operator!=(const Iterator &other) const { return !(*this == other); }
    private:
        Iterator(Node *current) : mCurrent(current) {}  // private constructor: only BinarySearchTree class can call it
        Node *mCurrent;
    };
    class ConstIterator
    {
    friend class BinarySearchTree<T>;
    public:
        ConstIterator(const Iterator &iterator) : mCurrent(iterator.mCurrent) {}  // public implicit conversion from Iterator

        const T &operator*() const { return mCurrent->mData; }
        const T *operator->() const { return &mCurrent->mData; }
        ConstIterator &operator++();
        ConstIterator operator++(int) { ConstIterator temp(*this); ++*this; return temp; }
        ConstIterator &operator--();
        ConstIterator operator--(int) { ConstIterator temp(mCurrent); --*this; return temp; }
        bool operator==(const ConstIterator &other) const { return mCurrent == other.mCurrent; }
        bool operator!=(const ConstIterator &other) const { return !(*this == other); }
    private:
        ConstIterator(Node *current) : mCurrent(current) {}
        
        Node *mCurrent;
    };
public:
    BinarySearchTree(const Function<bool(const T&, const T&)> &comparator = &Less<T>) : mRoot(nullptr), mNumElements(0), mComparator(comparator) {}

    ~BinarySearchTree() { Clear(); }

    size_t Size() const { return mNumElements; }
    bool Empty() const { return mNumElements == 0; }

    void Clear() { while (!Empty()) Remove(Begin()); }

    Iterator Begin();
    ConstIterator Begin() const;
    ConstIterator CBegin() const { return Begin(); }
    Iterator End() { return Iterator(nullptr); }
    ConstIterator End() const { return Iterator(nullptr); }
    ConstIterator CEnd() const { return End(); }

    /**** insertion is O(logN) if tree is balanced ****/
    template <typename U>
    void Insert(U &&element);
    template <typename U>
    void InsertRecursive(U &&element) { InsertRecursiveHelper(std::forward<U>(element), mRoot, nullptr); }

    /**** removal is O(logN) if tree is balanced ****/
    void Remove(const T &key);
    void Remove(const ConstIterator &iterator);

    /**** binary search is O(logN) if tree is balanced ****/
    Iterator Find(const T &key) { return FindHelper(key, mRoot); }
private:
    Node *mRoot;
    size_t mNumElements;

    Function<bool(const T&, const T&)> mComparator;

    template <typename U>
    void InsertRecursiveHelper(U &&element, Node *&node, Node *parent);

    Iterator FindHelper(const T &key, Node *root);
};

/**** binary search tree iterator class implementation ****/

template <typename T>
typename BinarySearchTree<T>::Iterator &BinarySearchTree<T>::Iterator::operator++()
{
    // find inorder successor
    if (mCurrent->mRightChild)  // has right sub tree
    {
        mCurrent = mCurrent->mRightChild;
        
        while (mCurrent->mLeftChild)  // find (leftmost) node of right sub-tree with smallest key
            mCurrent = mCurrent->mLeftChild;
    }
    // else
    // {
    //     Node *parent = mCurrent->mParent;
        
    //     if (parent) // not root
    //     {
    //         while (mCurrent == parent->mRightChild)
    //         {
    //             mCurrent = parent;
    //             parent = parent->mParent;

    //             if (!parent)  // reached root
    //                 break;
    //         }

    //         mCurrent = parent;
    //     }
    //     else   // root
    //         mCurrent = nullptr;  // off the end iterator
    // }
    else   // no right sub-tree
    {
        Node *temp;

        do  // go up 
        {
            temp = mCurrent;
            mCurrent = mCurrent->mParent;
        } while (mCurrent && temp == mCurrent->mRightChild);
    }

    return *this;
}

template <typename T>
typename BinarySearchTree<T>::Iterator &BinarySearchTree<T>::Iterator::operator--()
{
    // find inorder predecessor
    if (mCurrent->mLeftChild)   // has left sub-tree
    {
        mCurrent = mCurrent->mLeftChild;    // find (rightmost) node of left sub-tree with greatest key

        while (mCurrent->mRightChild)
            mCurrent = mCurrent->mRightChild;
    }
    else   // no left sub-tree
    {
        Node *temp = nullptr;

        do   // go up 
        {
            temp = mCurrent;
            mCurrent = mCurrent->mParent;
        } while (mCurrent && temp == mCurrent->mLeftChild);
    }

    return *this;    
}

template <typename T>
typename BinarySearchTree<T>::ConstIterator &BinarySearchTree<T>::ConstIterator::operator++()
{
    // find inorder successor
    if (mCurrent->mRightChild)  // has right sub tree
    {
        mCurrent = mCurrent->mRightChild;
        
        while (mCurrent->mLeftChild)  // find (leftmost) node of right sub-tree with smallest key
            mCurrent = mCurrent->mLeftChild;
    }
    // else
    // {
    //     Node *parent = mCurrent->mParent;
        
    //     if (parent) // not root
    //     {
    //         while (mCurrent == parent->mRightChild)
    //         {
    //             mCurrent = parent;
    //             parent = parent->mParent;

    //             if (!parent)  // reached root
    //                 break;
    //         }

    //         mCurrent = parent;
    //     }
    //     else   // root
    //         mCurrent = nullptr;  // off the end iterator
    // }
    else  // no right sub-tree
    {
        Node *temp;

        do  // go up 
        {
            temp = mCurrent;
            mCurrent = mCurrent->mParent;
        } while (mCurrent && temp == mCurrent->mRightChild);
    }

    return *this;
}

template <typename T>
typename BinarySearchTree<T>::ConstIterator &BinarySearchTree<T>::ConstIterator::operator--()
{
    // find inorder predecessor
    if (mCurrent->mLeftChild)   // has left sub-tree
    {
        mCurrent = mCurrent->mLeftChild;    // find (rightmost) node of left sub-tree with greatest key

        while (mCurrent->mRightChild)
            mCurrent = mCurrent->mRightChild;
    }
    else  // no left sub-tree
    {
        Node *temp = nullptr;

        do   // go up 
        { 
            temp = mCurrent;
            mCurrent = mCurrent->mParent;
        } while (mCurrent && temp == mCurrent->mLeftChild);
    }

    return *this;    
}

/**** binary search tree class implementation ****/

template <typename T>
typename BinarySearchTree<T>::Iterator BinarySearchTree<T>::Begin()
{   
    if (Empty())
        return End();

    Node *current = mRoot;

    while (current->mLeftChild)
        current = current->mLeftChild;

    return Iterator(current);
}

template <typename T>
typename BinarySearchTree<T>::ConstIterator BinarySearchTree<T>::Begin() const
{
    if (Empty())
        return End();
        
    Node *current = mRoot;

    while (current->mLeftChild)
        current = current->mLeftChild;

    return ConstIterator(current);
}

template <typename T>
template <typename U>
void BinarySearchTree<T>::Insert(U &&element)
{
    Node *newNode = new Node{ std::forward<U>(element), nullptr, nullptr, nullptr };

    if (Empty())
        mRoot = newNode;
    else
    {
        Node *child = mRoot, *parent = nullptr;

        bool isLeftChild = false;

        while (child)
        {
            parent = child;

            if (mComparator(element, parent->mData))  // key < current node's key
            {
                child = parent->mLeftChild;
                isLeftChild = true;
            }
            else                                       // key >= current node's key
            {
                child = parent->mRightChild; 
                isLeftChild = false;
            }
        }

        if (isLeftChild)
            parent->mLeftChild = newNode; 
        else
            parent->mRightChild = newNode;   

        newNode->mParent = parent;
    }

    mNumElements++;
}

template <typename T>
template <typename U>
void BinarySearchTree<T>::InsertRecursiveHelper(U &&element, Node *&node, Node *parent)
{
    if (!node)  // base case
    {
        node = new Node{ std::forward<U>(element), parent, nullptr, nullptr };
        mNumElements++;
    }
    // recursive cases
    else if (mComparator(element, node->mData))   // element < current node's key
        InsertRecursiveHelper(std::forward<U>(element), node->mLeftChild, node);
    else                                          // element >= current node's key
        InsertRecursiveHelper(std::forward<U>(element), node->mRightChild, node);
}

template <typename T>
void BinarySearchTree<T>::Remove(const T &key)
{
    if (Empty())
        return;

    Iterator it = Find(key);

    if (it != End())
        Remove(it);
}

template <typename T>
void BinarySearchTree<T>::Remove(const ConstIterator &iterator)
{
    if (Empty())
        return;

    Node *current = iterator.mCurrent;

    if (!current->mLeftChild && !current->mRightChild)  // node is external (leaf)
    {
        if(current == mRoot)
            mRoot = nullptr;
        else if (current == current->mParent->mLeftChild)
            current->mParent->mLeftChild = nullptr;
        else // current == current->mParent->mRightChild
            current->mParent->mRightChild = nullptr;
    }
    else if (!current->mLeftChild)     // node has one (right) child
    {
        if (current == mRoot)
        {
            mRoot = current->mRightChild;
            current->mParent = nullptr;
        }
        else if (current == current->mParent->mLeftChild)
            current->mParent->mLeftChild = current->mRightChild;
        else     // current == current->mParent->mRightChild
            current->mParent->mRightChild = current->mRightChild;

        current->mRightChild->mParent = current->mParent;
    } 
    else if (!current->mRightChild)     // node has one (left) child   
    {
        if (current == mRoot)
        {
            mRoot = current->mLeftChild;
            current->mParent = nullptr;
        }
        else if (current == current->mParent->mLeftChild)
            current->mParent->mLeftChild = current->mLeftChild;
        else     // current == current->mParent->mRightChild
            current->mParent->mRightChild = current->mLeftChild;

        current->mLeftChild->mParent = current->mParent;
    }
    else      // node has two children
    {
        // find following node in inorder traversal
        Node *inOrderNext = current->mRightChild;

        while (inOrderNext->mLeftChild)
            inOrderNext = inOrderNext->mLeftChild;

        if (inOrderNext == current->mRightChild)    // following node in inorder traversal is right child
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
        else   // folllowing node in inorder traversal is not right child
        {
            inOrderNext->mParent->mLeftChild = inOrderNext->mRightChild;  // unlink in-order successor

            if (inOrderNext->mRightChild)    // in-order successor has right child
                inOrderNext->mRightChild->mParent = inOrderNext->mParent;  // link to inorder successor's parent

            inOrderNext->mLeftChild = current->mLeftChild;   // link inorder successor in current node's place
            current->mLeftChild->mParent = inOrderNext;
            inOrderNext->mRightChild = current->mRightChild;
            current->mRightChild->mParent = inOrderNext;

            if (current == mRoot)
                mRoot = inOrderNext;
            else if (current == current->mParent->mLeftChild)
                current->mParent->mLeftChild = inOrderNext;
            else    // current == current->mParent->mRightChild
                current->mParent->mRightChild = inOrderNext;

            inOrderNext->mParent = current->mParent;
        }
    }
        
    delete current;

    mNumElements--;
}

template <typename T>
typename BinarySearchTree<T>::Iterator BinarySearchTree<T>::FindHelper(const T &key, Node *root)
{
    // non recursive binary search implementation
    Node *current = root;

    while (current)
    {
        if (mComparator(key, current->mData))       // key < current node's key
            current = current->mLeftChild;
        else if (mComparator(current->mData, key))  // key > current node's key
            current = current->mRightChild;
        else                                         // key == current node's key        
            break;
    }

    return Iterator(current);

    // recursive binary search implementation
    // if (key == root->mData)
    //     return Iterator(root);
    // else if (mComparator(key, root->mData))
    // {
    //     if (!root->mLeftChild)
    //         return End();

    //     return FindHelper(key, root->mLeftChild);
    // }
    // else
    // {
    //     if (!root->mRightChild)
    //         return End();

    //     return FindHelper(key, root->mRightChild);
    // }
}

#endif  // BINARY_SEARCH_TREE_H