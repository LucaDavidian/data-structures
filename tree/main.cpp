#include "binary_search_tree.hpp"
#include "tree.hpp"
#include <iostream>

#define PRINT(s)     std::cout << (s)
#define PRINTLN(s)   PRINT(s) << std::endl

template <typename T>
void PrintNode(typename Tree<T>::Node &node)
{
    PRINTLN(*node);
}

int main(int argc, char *argv[])
{
    Tree<int> ti;

    Tree<int>::Node *n1 = new Tree<int>::Node(10);
    n1->AddChild(new Tree<int>::Node(1));
    
    Tree<int>::Node *n2(new Tree<int>::Node(2));
    n2->AddChild(new Tree<int>::Node(0));
    n2->AddChild(new Tree<int>::Node(2));
    n2->AddChild(new Tree<int>::Node(3));

    n1->AddChild(n2);

    n1->AddChild(new Tree<int>::Node(7));

    PRINTLN(Depth<int>(n2));
    PRINTLN(Height<int>(*n1));
    ti.SetRoot(n1);
    PRINTLN(ti.Size());

    PRINTLN("preorder (recursive):");   
    ti.Preorder(PrintVisitor<int>());

    PRINTLN("preorder (stack - non recursive)");
    Preorder(n1, PrintVisitor<int>());

    PRINTLN("preorder (stack - non recursive)");
    Preorder2(n1, PrintVisitor<int>());
    
    PRINTLN("postorder (recursive):");
    ti.Postorder(PrintVisitor<int>());

    PRINTLN("postorder (stack - non recursive):");
    Postorder(n1, PrintVisitor<int>());

    PRINTLN("postorder (stack - non recursive):");
    Postorder2(n1, PrintVisitor<int>());

    PRINTLN("level-order (breadth-first):");
    BreadthFirst(n1, PrintVisitor<int>());

    n1->RemoveChild(n2);

    PRINTLN("postorder (recursive):");

    ti.Postorder(&PrintNode<int>);



    // BinarySearchTree<int> ti;

    // ti.Insert(10);
    // ti.Insert(11);
    // ti.Insert(12);
    // ti.Insert(14);
    // ti.Insert(16);
    // ti.Insert(17);
    // ti.Insert(100);

    // ti.Display();

    // ti.Delete(17);
    // ti.Delete(10);

    // ti.Display();

    // std::cout << *ti.Find(12) << std::endl;

    return 0;
}