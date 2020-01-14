#ifndef TREE_H
#define TREE_H

#include "../list/double_ended_doubly_linked_list.hpp"
#include <cstddef>
#include <iostream>

using std::size_t;

template <typename T>
class Tree;

template <typename T>
class Visitor
{
public:
    virtual void Visit(typename Tree<T>::Node&) const = 0;
    virtual void operator()(typename Tree<T>::Node&) const = 0;
};

template <typename T>
class PrintVisitor : public Visitor<T>
{
public:
    void Visit(typename Tree<T>::Node &node) const override { std::cout << *node << std::endl; }
    void operator()(typename Tree<T>::Node &node) const override { Visit(node); }
};

template <typename T>
class CountVisitor : public Visitor<T>
{
public:
    void Visit(typename Tree<T>::Node &node) const override { mCount++; }
    void operator()(typename Tree<T>::Node &node) const override { Visit(node); }
    size_t GetCount() { return mCount; }
private:
    mutable size_t mCount = 0;
};

template <typename T>
class Tree
{
public:
    class Node
    {
        public:
            template <typename U>
            Node(U &&data) : mData(std::forward<U>(data)), mParent(nullptr) {}
            ~Node() { for (typename DoublyLinkedList<Node*>::Iterator it = mChildren.Begin(); it != mChildren.End(); ++it) delete *it;}
            
            T &operator*() { return const_cast<T&>(*static_cast<const Node&>(*this)); }
            const T &operator*() const { return mData; }
            const Node *GetParent() const { return mParent; }
            const DoublyLinkedList<Node*> *GetChildren() const { return &mChildren; }
            
            bool IsRoot() const { return !mParent; }
            bool IsExternal() const { return mChildren.Empty(); }
            
            void AddChild(Node *child) { mChildren.InsertLast(child); child->mParent = this; }
            void RemoveChild(Node *child) { auto it = mChildren.Find(child); delete *it;  mChildren.Remove(it); }

            template <typename U>
            void Accept(const U &visitor) { visitor(*this); }
            //void Accept(const Visitor<T> &visitor) { visitor.Visit(*this); }
        private:
            T mData;
            Node *mParent;
            DoublyLinkedList<Node*> mChildren;  
    };
    using NodeList = DoublyLinkedList<Node*>;   
public:
    Tree() : mRoot(nullptr) {}
    ~Tree() { }

    bool Empty() const { return !mRoot; }
    // size_t Size() const { mCount = 1; Count(mRoot); return mCount; }
    size_t Size() { CountVisitor<T> count; Preorder_(*mRoot, count); return count.GetCount(); }
    void SetRoot(Node *root) { mRoot = root; }

    template <typename U>
    void Preorder(const U &visitor) { Preorder_(*mRoot, visitor); }
    template <typename U>
    void Postorder(const U &visitor) { Postorder_(*mRoot, visitor); }
    // void Preorder(const Visitor<T> &visitor) { Preorder_(*mRoot, visitor); }
    // void Postorder(const Visitor<T> &visitor) { Postorder_(*mRoot, visitor); }
private:
    template <typename U>
    void Preorder_(Node &root, const U &visitor);
    template <typename U>
    void Postorder_(Node &root, const U &visitor);
    // void Preorder_(Node &root, const Visitor<T> &visitor);
    // void Postorder_(Node &root, const Visitor<T> &visitor);
    // mutable size_t mCount = 0;
    // void Count(Node *root) const { mCount += root->GetChildren()->Size(); for (typename NodeList::Iterator it = root->GetChildren()->Begin(); it != root->GetChildren()->End(); ++it) Count(*it); }
    Node *mRoot;
};

template <typename T>
template <typename U>
void Tree<T>::Preorder_(Node &root, const U &visitor)
{
    root.Accept(visitor);
    
    for (typename Tree<T>::NodeList::Iterator it = root.GetChildren()->Begin(); it != root.GetChildren()->End(); ++it)
        Preorder_(**it, visitor);
}

template <typename T>
template <typename U>
void Tree<T>::Postorder_(Node &root, const U &visitor)
{    
    for (typename Tree<T>::NodeList::Iterator it = root.GetChildren()->Begin(); it != root.GetChildren()->End(); ++it)
        Postorder_(**it, visitor);

    root.Accept(visitor);
}

/**** helper functions ****/

// depth of a node
template <typename T>
size_t Depth(const typename Tree<T>::Node *node)  // qualified type names are not deduced contexts
{
    if (node->IsRoot())
        return 0;
    else
        return 1 + Depth<T>(node->GetParent());
}

// height of a node
template <typename T>
size_t Height(const typename Tree<T>::Node &node)
{
    if (node.IsExternal())
        return 0;
    else
    {
        size_t h = 0;
        for (typename Tree<T>::NodeList::Iterator it = node.GetChildren()->Begin(); it != node.GetChildren()->End(); ++it)
        {
            size_t childHeight = Height<T>(**it);
            if (childHeight > h)
                h = childHeight;
        }

        return 1 + h;
    }
}

/**** tree traversal algorithms ****/

// preorder traversal - recursive implementation
template <typename T>
void Preorder(typename Tree<T>::Node &root, const Visitor<T> &visitor)
{
    root.Accept(visitor);
    
    for (typename Tree<T>::NodeList::Iterator it = root.GetChildren()->Begin(); it != root.GetChildren()->End(); ++it)
        Preorder<T>(**it, visitor);
}

#include "../ADT/stack/stack.hpp"

// preorder traversal - iterative (non recursive) implementation
template <typename T, typename F>
void Preorder(T &root, const F &visitor)
{       
    Stack<typename DoublyLinkedList<T>::Iterator> iteratorStack;
    Stack<typename DoublyLinkedList<T>::Iterator> endStack;

    typename DoublyLinkedList<T>::Iterator begin = root->GetChildren()->Begin(); 
    iteratorStack.Push(begin);

    typename DoublyLinkedList<T>::Iterator end = root->GetChildren()->End(); 
    endStack.Push(end);

    root->Accept(visitor);

    while (!iteratorStack.Empty())
    {
        if (iteratorStack.Top() == endStack.Top())
        {
            iteratorStack.Pop();
            endStack.Pop();

            if (!iteratorStack.Empty())
                ++iteratorStack.Top();

            continue;
        }

        (*iteratorStack.Top())->Accept(visitor);

        if (!(*iteratorStack.Top())->IsExternal())
        {
            endStack.Push((*iteratorStack.Top())->GetChildren()->End());
            iteratorStack.Push((*iteratorStack.Top())->GetChildren()->Begin());
        }
        else
            ++iteratorStack.Top();
    }
}

// preorder traversal - iterative (non recursive) implementation
template <typename T, typename F>
void Preorder2(T &root, const F &visitor)
{
    Stack<T> stack;

    stack.Push(root);

    while (!stack.Empty())
    {
        T node = stack.Top();
        
        node->Accept(visitor);
        stack.Pop();

        // push children on stack (reverse order)
        DoublyLinkedList<T> reverse;
        for (typename DoublyLinkedList<T>::Iterator it = node->GetChildren()->Begin(); it != node->GetChildren()->End(); ++it)
            reverse.InsertFirst(*it);

        for (auto it = reverse.Begin(); it != reverse.End(); ++it)
            stack.Push(*it);
    }
}

// postorder traversal - recursive implementation
template <typename T>
void Postorder(typename Tree<T>::Node &root, const Visitor<T> &visitor)
{    
    for (typename Tree<T>::NodeList::Iterator it = root.GetChildren()->Begin(); it != root.GetChildren()->End(); ++it)
        Postorder<T>(**it, visitor);

    root.Accept(visitor);
}

// postorer traversal - iterative (non recursive) implementation
template <typename T, typename F>
void Postorder(T &root, const F &visitor)
{       
    Stack<typename DoublyLinkedList<T>::Iterator> iteratorStack;
    Stack<typename DoublyLinkedList<T>::Iterator> endStack;

    typename DoublyLinkedList<T>::Iterator begin = root->GetChildren()->Begin(); 
    iteratorStack.Push(begin);

    typename DoublyLinkedList<T>::Iterator end = root->GetChildren()->End(); 
    endStack.Push(end);

    while (!iteratorStack.Empty())
    {
        if (iteratorStack.Top() == endStack.Top())
        {
            iteratorStack.Pop();
            endStack.Pop();

            if (!iteratorStack.Empty())
            {
                (*iteratorStack.Top())->Accept(visitor);
                ++iteratorStack.Top();
            }   
            
            continue;
        }

        if (!(*iteratorStack.Top())->IsExternal())
        {
            endStack.Push((*iteratorStack.Top())->GetChildren()->End());
            iteratorStack.Push((*iteratorStack.Top())->GetChildren()->Begin());
        }
        else
        {
            (*iteratorStack.Top())->Accept(visitor);
            ++iteratorStack.Top();
        }        
    }

    root->Accept(visitor);
}

// postorer traversal - iterative (non recursive) implementation
template <typename T, typename F>
void Postorder2(T &root, const F &visitor)
{
    Stack<T> stack;
    Stack<T> reverse;

    stack.Push(root);

    while (!stack.Empty())
    {
        T node = stack.Top();
        
        reverse.Push(node);
        stack.Pop();

        DoublyLinkedList<T> reverse;
        for (typename DoublyLinkedList<T>::Iterator it = node->GetChildren()->Begin(); it != node->GetChildren()->End(); ++it)
            stack.Push(*it);   
    }

    while (!reverse.Empty())
    {
        T node = reverse.Top();
        node->Accept(visitor),
        reverse.Pop();
    }
}

#include "../ADT/queue/queue.hpp"

// level-order (breadth first) traversal
template <typename T, typename F>
void BreadthFirst(T &root, const F &visitor)
{
    Queue<T> queue;

    queue.Enqueue(root);

    while (!queue.Empty())
    {
        T node = queue.Front();
        queue.Dequeue();
        node->Accept(visitor);

        for (auto it = node->GetChildren()->Begin(); it != node->GetChildren()->End(); ++it)
            queue.Enqueue(*it);
    }    
}

#endif  // TREE_H