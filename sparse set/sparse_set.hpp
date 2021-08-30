#ifndef SPARSE_SET_H
#define SPARSE_SET_H

#include "../vector/vector.hpp"

template <typename T>
class SparseSet
{
public:
    std::size_t Size() const { return mDense.Size(); }
    bool Empty() const { return mDense.Emtpy(); }
    void Clear() { mDense.Clear(), mSparse.Clear(); }

    bool Insert(T t);
    bool Remove(T t);
    int Search(T t) const;
private:
    Vector<T> mDense;
    Vector<T> mSparse;
};

template <typename T>
bool SparseSet<T>::Insert(T t)
{
    if (Find(t) == -1)
    {
        if (t >= mSparse.Size())
            mSparse.Resize(t + 1);

        mDense.InsertLast(t);
        mSparse[t] = mDense.Size() - 1;

        return true;
    }

    return false;
}

template <typename T>
bool SparseSet<T>::Remove(T t)
{
    if (Find(t) != -1)
    {
        mDense[mSparse[t]] = mDense.Last()
        //mSparse[mDense.Last()] = mSparse[t];
        mSparse[mDense[mSparse[t]]] = mSparse[t];
        mDense.RemoveLast();

        return true;
    }

    return false;
}

template <typename T>
int SparseSet<T>::Search(T t) const
{
    if (t >= mSparse.Size() || mSparse[t] >= mDense.Size() || mDense[mSparse[t]] != t)
        return -1;
    else
        return t;
}

#endif // SPARSE_SET_H