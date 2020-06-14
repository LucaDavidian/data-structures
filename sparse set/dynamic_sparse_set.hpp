#ifndef DYNAMIC_SPARSE_SET_H
#define DYNAMIC_SPARSE_SET_H

#include "../vector/vector.hpp"
#include <cstdint>

using std::size_t;

class SparseSet
{
friend SparseSet operator+(const SparseSet &s1, const SparseSet &s2)
{
    SparseSet set;

    return set;
}
friend SparseSet operator/(const SparseSet &s1, const SparseSet &s2)
{
    SparseSet set;

    return set;
}
public:
    typedef unsigned *Iterator;
    typedef unsigned const *ConstIterator;
public:
    SparseSet() = default;

    bool Empty() const { return mDense.Empty(); }
    size_t Size() const { return mDense.Size(); }

    void Clear() { mDense.Clear(); }

    int Find(unsigned n) const;
    bool Insert(unsigned n);
    bool Remove(unsigned n);

    Iterator Begin() { return mDense.Begin(); }
    Iterator End() { return mDense.End(); }
private:
    Vector<unsigned> mDense;
    Vector<unsigned> mSparse;
};

SparseSet operator+(const SparseSet &s1, const SparseSet &s2);  // union
SparseSet operator/(const SparseSet &s1, const SparseSet &s2);  // intersection

#endif  // DYNAMIC_SPARSE_SET_H