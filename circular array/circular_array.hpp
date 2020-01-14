#ifndef CIRCULAR_ARRAY_H
#define CIRCULAR_ARRAY_H

#include <cstddef>
#include <utility>
#include <exception>

using std::size_t;

template <typename T>
class CircularArray
{
public:
    CircularArray(size_t size = 10);
    ~CircularArray() { Clear(); operator delete(mBuffer); }

    bool Empty() const { return mHead == mTail; }
    bool Full() const { size_t temp = (mTail + 1) % mSize; return temp == mHead; /* return Size() == mSize - 1;*/ } 
    size_t Size() const { return mHead <= mTail ? mTail - mHead : mSize - (mHead - mTail); }

    template <typename U>
    void InsertFirst(U&&);
    template <typename U>
    void InsertLast(U&&);

    void RemoveFirst();
    void RemoveLast();

    void Clear();

    T &First() { return const_cast<T&>(static_cast<const CircularArray*>(this)->First()); }
    const T &First() const { if (Empty()) throw std::exception(); return mBuffer[mHead]; }
    T &Last() { return const_cast<T&>(static_cast<const CircularArray*>(this)->Last()); }
    const T &Last() const { if (Empty()) throw std::exception(); return mBuffer[mTail - 1]; }
private:
    T *mBuffer;
    size_t mSize;
    size_t mHead;
    size_t mTail;
};

template <typename T>
CircularArray<T>::CircularArray(size_t size) : mSize(size), mHead(0), mTail(0)
{
    void *rawMem = operator new(sizeof(T) * size);

    mBuffer = static_cast<T*>(rawMem);
}

template <typename T>
template <typename U>
void CircularArray<T>::InsertFirst(U &&element)
{
    if (Full())
        throw std::exception();

    if (mHead == 0)
        mHead = mSize - 1;
    else
        mHead--;

    new(&mBuffer[mHead]) T(std::forward<U>(element));  // placement-new
}
    
template <typename T>
template <typename U>
void CircularArray<T>::InsertLast(U &&element)
{
    if (Full())
        throw std::exception();

    new(&mBuffer[mTail]) T(std::forward<U>(element));   // placement-new

    mTail++;
    mTail %= mSize;
}

template <typename T>
void CircularArray<T>::RemoveFirst()
{
    if (Empty())
        throw std::exception();

    mBuffer[mHead].~T();

    mHead++;
    mHead %= mSize;
}
    
template <typename T>
void CircularArray<T>::RemoveLast()
{
    if (Empty())
        throw std::exception();

    if (mTail == 0)
        mTail = mSize - 1;
    else
        mTail--;

    mBuffer[mTail].~T();
}

template <typename T>
void CircularArray<T>::Clear()
{
    while (!Empty())
        RemoveFirst();
}

#endif  // CIRCULAR_ARRAY_H