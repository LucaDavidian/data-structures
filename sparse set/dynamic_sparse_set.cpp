#include "dynamic_sparse_set.hpp"

int SparseSet::Find(unsigned n) const
{
    if (n + 1 > mSparse.Size())
        return -1;

    if (mSparse[n] + 1> mDense.Size() || mDense[mSparse[n]] != n)
        return -1;

    return mSparse[n];
}

bool SparseSet::Insert(unsigned n)
{
    if (Find(n) == -1)
    {
        if (n + 1 > mSparse.Size())
            mSparse.Resize(n + 1);

        mDense.InsertLast(n);
        mSparse[n] = mDense.Size() - 1;

        return true;
    }
    
    return false;
}

bool SparseSet::Remove(unsigned n)
{
    if (Find(n) == -1)
        return false;

    mDense[mSparse[n]] = mDense.Last();
    mSparse[mDense.Last()] = mSparse[n];
    mDense.RemoveLast();

    return true;
}