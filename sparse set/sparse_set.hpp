#ifndef SPARSE_SET
#define SPARSE_SET

#include <cassert>

/* a set (no duplicates) of n unsigned integers from a universe of all unsigned integers in 0 -> N */
template <unsigned N>
class SparseSet
{
public:
    SparseSet();

    bool Insert(unsigned n);
    bool Remove(unsigned n);
    int Find(unsigned n);
    void Clear() { mCount = 0; }
private:
    unsigned mDense[N + 1];
    unsigned mSparse[N + 1];
    unsigned int mCount;
};  

template <unsigned N>
SparseSet<N>::SparseSet() : mCount(0) 
{
    for (int i = 0; i < N + 1; i++)
        mSparse[i] = 0U;
}

template <unsigned N>
bool SparseSet<N>::Insert(unsigned n)
{
    assert(n <= N);

    if (Find(n) != -1)
        return false;   // no duplicates

    mDense[mCount] = n;
    mSparse[n] = mCount;
    mCount++;

    return true;
}

template <unsigned N>
bool SparseSet<N>::Remove(unsigned n)
{
    assert(n <= N);

    if (mSparse[n] >= mCount || Find(n) == -1)
        return false;

    mDense[mSparse[n]] = mDense[mCount - 1];
    mSparse[mDense[mCount - 1]] = mSparse[n];   //mSparse[mDense[mSparse[[n]]] = mSparse[n];
    mCount--;

    return true;
}

template <unsigned N>
int SparseSet<N>::Find(unsigned n)
{
    assert(n <= N);

    if (mSparse[n] >= mCount)
        return -1;

    unsigned int denseIndex = mSparse[n];

    if (mDense[denseIndex] == n)  
        return denseIndex;
    
    return -1;
}

#endif // SPARSE_SET