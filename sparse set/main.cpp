//#include "sparse_set.hpp"
#include "dynamic_sparse_set.hpp"
#include <random>
#include <iostream>

int main(int argc, char **argv)
{
    SparseSet s;

    std::uniform_int_distribution<unsigned> distr(0, 1000);
    std::default_random_engine randEng;

    for (int i = 0; i < 10000; i++)
        s.Insert(distr(randEng));

    std::cout << "is 5 present: " << s.Find(5) << std::endl;
    std::cout << "is 120 present: " << s.Find(120) << std::endl;

    s.Remove(3);
    s.Remove(1);

    std::cout << "is 10 present: " << s.Find(10) << std::endl;
    std::cout << "is 120 present: " << s.Find(120) << std::endl;

    for (SparseSet::Iterator it = s.Begin(); it != s.End(); ++it)
        std::cout << *it << "  ";

    return 0;
}