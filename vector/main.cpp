#include "vector.hpp"
#include <string>

#include "../bubble_sort.hpp"
#include "../print_sequence.hpp"
#include "../sequence_reverse.hpp"

class MyClass
{
public:
    MyClass(int a = 100, double b = 0.1) : i(a), d(b) {}
    int GetInt() const { return i; }
    double GetDouble() const { return d; } 
private:
    int i;
    double d;
};

#include <iostream>

std::ostream &operator<<(std::ostream &os, const MyClass &mc)
{
    return os << mc.GetInt() << " " << mc.GetDouble();
}

int main(int argc, char *argv[])
{
    Vector<int> vi;

    vi.InsertLast(100);
    vi.InsertFirst(11);
    vi.Insert(2, 40);
    vi.Insert(1, 99);

    auto it = vi.CBegin();

    ITPrintSequence(vi);

    IT_BubbleSort(vi);
    // BubbleSort(vi);

    PrintSequence(vi);

    SequenceReverse(vi);

    PrintSequence(vi);

    vi.Remove(1);
    vi.Remove(1);

    PrintSequence(vi);

    PRINTLN("");

    Vector<std::string> vs;

    vs.InsertLast("world");
    vs.InsertFirst("hello");
    vs.Insert(2, "!");
    vs.Insert(1, "-");

    PrintSequence(vs);

    vs.Remove(1);
    vs.Remove(1);

    PrintSequence(vs);

    vs.RemoveFirst();

    PrintSequence(vs);

    vs.RemoveLast();

    PrintSequence(vs);

    try
    {
        vs.RemoveLast();
    }
    catch (std::exception)
    {
        PRINTLN("exception caught! vector empty!");
    }

    PRINTLN("");

    Vector<float> vf;

    vf.InsertFirst(1.0f);
    vf.InsertLast(0.2f);
    vf.Insert(1, 2.3f);

    PrintSequence(vf);

    Vector<float> vf2 = vf;

    PrintSequence(vf2);

    Vector<int> vi2 = std::move(vi);

    PrintSequence(vi2);

    vi = vi2;

    PrintSequence(vi);

    Vector<std::string> vs2;

    vs2.InsertLast("world");
    vs2.InsertFirst("hello");
    vs2.Insert(vs2.End(), "_!");

    PrintSequence(vs2);

    SequenceReverse(vs2);

    PrintSequence(vs2);

    vs2.Remove(vs2.Begin());

    PrintSequence(vs2);

    vs2.RemoveFirst();

    PrintSequence(vs2);

    vs2.RemoveLast();

    PrintSequence(vs2);

    Vector<MyClass> vmc;

    vmc.InsertFirst(MyClass(1, 1.3));
    vmc.InsertLast(MyClass(12, 12.3));
    vmc.Emplace(0, 22, 33.2);  
    vmc.Emplace(vmc.Begin(), 12, 0.33); 
    vmc.EmplaceLast(44, 44.3);

    PrintSequence(vmc);

    vmc.Remove(vmc.AtIndex(1), vmc.End());  

    PrintSequence(vmc);

    vmc.Resize(10);

    PrintSequence(vmc);

    vmc.Resize(1);

    PrintSequence(vmc);

    vmc.Resize(5);

    for (int i = 0; i < 5; i++)
        vmc.Data()[i] = MyClass(i, i * 2);

    PrintSequence(vmc);

    for (MyClass &mc : vmc)
        std::cout << mc << std::endl;

    std::cout << "done" << std::endl;

    Vector<MyClass> myCV = { 1, 3, 4, 5 };

    for (MyClass &mc : myCV)
        std::cout << mc << std::endl;

    std::cout << "done" << std::endl;

    Vector<int> vint = {1, 3, 4, 6, 72, 442};

    for (int i : vint)
        std::cout << i << " ";
    std::cout << std::endl;

    Vector<float> vff = {1.2f, 3.3f, 0.5f};

    vint.Insert(vint.AtIndex(3), vff.Begin(), vff.End());

    for (int i : vint)
        std::cout << i << " ";
    std::cout << std::endl;

    return 0;
}