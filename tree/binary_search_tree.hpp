#ifndef BS_TREE_H
#define BS_TREE_H

#include <cstddef>
#include <cassert>
#include <utility>
#include <iostream>

using std::size_t;

template <typename T>
class BinarySearchTree
{
private:
    struct Node
    {
        T data;
        Node *parent;
        Node *leftChild;
        Node *rightChild;
    };
public:
    class Position
    {
    friend class BinarySearchTree;
    public:
        Position(Node *node) : mCurrent(node) {}

        T &operator*() { return mCurrent->data; }
        T *operator->() { return &mCurrent->data; }
        Position Left() { return mCurrent->leftChild; }
        Position Right() { return mCurrent->rightChild; }
    private:
        Node *mCurrent;
    };
public:
    BinarySearchTree() : mRoot(nullptr) {}
    ~BinarySearchTree() = default;

    size_t Size() const { return mNumElements; }
    bool Empty() const { return !mRoot; }

    template <typename U>
    void Insert(U&&);
    Position Find(const T&);
    void Delete(const T&);

    void PreOrder(Node *root);
    void PostOrder(Node *root);
    void InOrder(Node *root);
    void LevelOrder(Node *root);

    void Display() { InOrder(mRoot); }
private:
    Node *mRoot;
    size_t mNumElements;
};

template <typename T>
typename BinarySearchTree<T>::Position BinarySearchTree<T>::Find(const T &key)
{
    assert(!Empty());

    Node *current = mRoot;

    while (current && key != current->data)
        if (key < current->data)
            current = current->leftChild;
        else
            current = current->rightChild;
        
    return Position(current);
}

template <typename T>
template <typename U>
void BinarySearchTree<T>::Insert(U &&element)
{
    Node *newNode = new Node{std::forward<U>(element), nullptr, nullptr};
    
    /*** alternative algorithm implementation 1 ****/
    // Node *current = mRoot, *parent = nullptr;
    
    // while (current)
    // {
    //     parent = current;
    //     if (element < current->data)
    //         current = current->leftChild;
    //     else
    //         current = current->rightChild;
    // }
       
    // if (parent == nullptr)
    //     mRoot = newNode;
    // else
    //     if (element < parent->data)   // not necessary with bool isLeftChild
    //         parent->leftChild = newNode;
    //     else
    //         parent->rightChild = newNode; 


    /**** alternative algorithm implementation 1 ****/
    if (Empty())
        mRoot = newNode;
    else
    {
        Node *current = mRoot;
        while (true)
        {
            if (element < current->data)
            {
                if (!current->leftChild)
                {
                    current->leftChild = newNode;
                    return;   // break;
                }
        
                current = current->leftChild;                
            }
            else  // element >= current->data
            {
                if (!current->rightChild)
                {
                    current->rightChild = newNode;
                    return;   // break;
                }
                
                current = current->rightChild;     
            }           
        }
    }
    
   
    /**** alternative algorithm implementation 3 ****/
    // if (!mRoot)
    //     mRoot = newNode;
    // else
    // {
    //     Node *current = mRoot, *parent = current;

    //     while (true)
    //     {
    //         parent = current;
    //         if (element < current->data)
    //         {
    //             current = current->leftChild;
    //             if (!current)
    //             {
    //                 parent->leftChild = newNode;
    //                 break;
    //             }
    //         }
    //         else
    //         {
    //             current = current->rightChild;
    //             if (!current)
    //             {
    //                 parent->rightChild = newNode;
    //                 break;
    //             }
    //         }
    //     }   
    // }
}

template <typename T>
void BinarySearchTree<T>::Delete(const T &key)
{
    if (Empty())
        return;
    
    // assert(!Empty());

    Node *current = mRoot, *parent = nullptr;
    bool isLeftChild = true;

    while (current && key != current->data)
    {
        parent = current;
        if (key < current->data)
        {
            current = current->leftChild;
            isLeftChild = true;
        }
        else
        {
            current = current->rightChild;
            isLeftChild = false;
        }
    }

    if (!current)
        return;  // not found

    //found
    if (!current->leftChild && !current->rightChild)  // no children
        if (!parent)    // node to be deleted is root
            mRoot = nullptr;
        else
            isLeftChild ? parent->leftChild = nullptr : parent->rightChild = nullptr;
    else if (!current->leftChild)   // only right child
        if (!parent)    // node to be deleted is root
            mRoot = current->rightChild;
        else
            isLeftChild ? parent->leftChild = current->rightChild : parent->rightChild = current->rightChild;
    else if (!current->rightChild)   // only left child
        if (!parent)    // node to be deleted is root
            mRoot = current->leftChild;
        else
            isLeftChild ? parent->leftChild = current->leftChild : parent->rightChild = current->leftChild;
    else   // two children
    {
        if (!parent)  // node to be deleted is root
        {
            Node *successor = current->rightChild, *successorParent = nullptr;
            while (successor->leftChild)
            {
                successorParent = successor;
                successor = successor->leftChild;
            }

            if (!successorParent)  // right child is successor
            {
                successor->leftChild = current->leftChild;   // mRoot->rightChild->leftChild = mRoot->leftChild;
                mRoot = successor;                           // mRoot = mRoot->rightChild;
            }
            else   // right child's left descendant is successor
            {
                successorParent->leftChild = successor->rightChild;
                successor->rightChild = successorParent;
                successor->leftChild = mRoot->leftChild;
                mRoot = successor;
            }
        }
        else  // node to be deleted is not root
        {
            Node *successor = current->rightChild, *successorParent = nullptr;
            while (successor->leftChild)
            {
                successorParent = successor;
                successor = successor->leftChild;
            }

            if (!successorParent)  // right child is successor
            {
                // current->rightChild->leftChild = current->leftChild;
                successor->leftChild = current->leftChild; 
                // isLeftChild ? parent->leftChild = current->rightChild : parent->rightChild = current->rightChild;
                isLeftChild ? parent->leftChild = successor : parent->rightChild = successor;
            }
            else   // right child's left descendant is successor
            {
                successorParent->leftChild = successor->rightChild;
                successor->rightChild = successorParent;
                successor->leftChild = current->leftChild;
                isLeftChild ? parent->leftChild = successor : parent->rightChild = successor;
            }
        }        
    }

    delete current;
}

template <typename T>
void BinarySearchTree<T>::InOrder(Node *root)
{
    if (root->leftChild)
        InOrder(root->leftChild);
    
    std::cout << root->data << std::endl;
    
    if (root->rightChild)
        InOrder(root->rightChild);
}

#endif  // BS_TREE_H
