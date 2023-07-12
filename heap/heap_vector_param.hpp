#ifndef HEAP_H
#define HEAP_H

#include <cstddef>
#include <exception>
#include <utility>
#include <new>

using std::size_t;

class HeapEmptyException : public std::exception {};

template <typename T>  // min heap by default
bool Less(const T &a, const T &b)
{
    return a < b;
}

/**** binary heap class vector implementation (using an additional template type parameter for comparator function object) ****/
template <typename T, typename F = decltype(&Less<T>)>
class Heap
{
public:
    Heap(const F &comparator = Less<T>) : mHeapArray(nullptr), mCapacity(0), mNumElements(0), mComparator(comparator) {}

    ~Heap() { for (size_t i = 0; i < mNumElements; i++) Remove(); operator delete(mHeapArray); }
    
    bool Empty() const { return mNumElements == 0; }
    
    size_t Size() const { return mNumElements; }
    
    size_t Capacity() const { return mCapacity; }

    void Reserve(size_t size);

    template <typename U>
    void Insert(U &&);

    void Remove();

    bool Remove(const T &element);


    const T &Peek() const { if (Empty()) throw HeapEmptyException(); return mHeapArray[0]; }

private:
    T *mHeapArray;
    size_t mCapacity;
    size_t mNumElements;
    F mComparator;

    size_t GetParentIndex(size_t index) const { return (index - 1) / 2; }
    size_t GetLeftChildIndex(size_t index) const { return 2 * index + 1; }
    size_t GetRightChildIndex(size_t index) const { return 2 * index + 2; }

    int Find(const T &element);
    
    void BubbleUpSwap(size_t index);
    void BubbleUpCopy(size_t index);
    void TrickleDownSwap(size_t index);
    void TrickleDownCopy(size_t index);
};

template <typename T, typename F>
void Heap<T,F>::Reserve(size_t size)
{
    // if requested capacity is less than actual capacity return
    if (size <= mCapacity)
        return;
    else
        mCapacity = size;

    // allocate new buffer (operator new)
    void *rawMem = operator new(size * sizeof(T));

    // copy elements to new buffer (placement-new)
    for (size_t i = 0; i < mNumElements; i++)
        new(&static_cast<T*>(rawMem)[i]) T(std::move(mHeapArray[i]));

    // destroy old elements
    for (size_t i = 0; i < mNumElements; i++)
        mHeapArray[i].~T();

    // deallocate old buffer (operator delete)
    operator delete(mHeapArray);

    // set new buffer
    mHeapArray = static_cast<T*>(rawMem);
}

template <typename T, typename F>
template <typename U>
void Heap<T,F>::Insert(U &&element)
{
    if (mCapacity <= mNumElements)
        Reserve(mCapacity ? mCapacity * 2 : 1);
    
    new(&mHeapArray[mNumElements]) T(std::forward<U>(element));

    mNumElements++;

    // bubble up element (heapify - restore heap condition)
    //BubbleUpSwap(mNumElements - 1);
    BubbleUpCopy(mNumElements - 1);
}

template <typename T, typename F>
void Heap<T,F>::Remove()
{
    if (Empty())
        throw HeapEmptyException();

    if (mNumElements == 0)
    {
        mHeapArray[0].~T();

        mNumElements--;
    }
    else
    {
        mHeapArray[0] = std::move(mHeapArray[mNumElements - 1]);
        mHeapArray[mNumElements - 1].~T();

        mNumElements--;

        // trickle down (heapify - restore heap condition)
        //TrickleDownSwap(0);
        TrickleDownCopy(0);
    }
}

template <typename T, typename F>
bool Heap<T, F>::Remove(const T &element)
{
    if (int index = Find(element); index != -1)
    {
        // old value <= last value (min heap) or old value >= last value (max heap)
        bool compare = mComparator(mHeapArray[index], mHeapArray[mNumElements - 1]) || !mComparator(mHeapArray[index], mHeapArray[mNumElements - 1]) && !mComparator(mHeapArray[mNumElements - 1], mHeapArray[index]);

        mHeapArray[index] = {std::move(mHeapArray[mNumElements - 1])};
        mHeapArray[mNumElements - 1].~T();

        mNumElements--;

        if (compare)
            TrickleDownCopy(index);
        else
            BubbleUpCopy(index);

        return true;
    }

    return false;
}

template <typename T, typename F>
int Heap<T, F>::Find(const T &element)
{
    for (size_t i = 0; i < mNumElements; i++)
        if (element == mHeapArray[i])
            return static_cast<int>(i);

    return -1;
}

template <typename T, typename F>
void Heap<T,F>::BubbleUpSwap(size_t index)
{
    // move the element up
    while (index > 0)
    {
        size_t parentIndex = GetParentIndex(index);

        // parent <= child (min heap) or parent >= child (max heap)
        if (mComparator(mHeapArray[parentIndex], mHeapArray[index]) || !mComparator(mHeapArray[parentIndex], mHeapArray[index]) && !mComparator(mHeapArray[index], mHeapArray[parentIndex]))
            return;
        
        // else swap parent and child
        T temp = std::move(mHeapArray[index]);
        mHeapArray[index] = std::move(mHeapArray[parentIndex]);
        mHeapArray[parentIndex] = std::move(temp);

        //using std::swap;
        //swap(mHeapArray[parentIndex], mHeapArray[childIndex]);

        index = parentIndex;
    }
}

template <typename T, typename F>
void Heap<T,F>::BubbleUpCopy(size_t index)
{
    // save the element
    T temp = std::move(mHeapArray[index]);

    // move the hole up
    while (index > 0)
    {
        size_t parentIndex = GetParentIndex(index);

        // parent <= child (min heap) or parent >= child (max heap)
        if (mComparator(mHeapArray[parentIndex], temp) || !mComparator(mHeapArray[parentIndex], temp) && !mComparator(temp, mHeapArray[parentIndex]))
            break;

        // else move the hole up
        mHeapArray[index] = std::move(mHeapArray[parentIndex]);

        index = parentIndex;
    }

    // fill the hole
    mHeapArray[index] = std::move(temp);
}

template <typename T, typename F>
void Heap<T,F>::TrickleDownSwap(size_t index)
{
    // move the element down
    while (GetLeftChildIndex(index) < mNumElements)  // node has at least left child
    {
        size_t leftChildIndex = GetLeftChildIndex(index);
        size_t rightChildIndex = GetRightChildIndex(index);

        size_t childIndex;
        
        if (rightChildIndex >= mNumElements)
            childIndex = leftChildIndex;
        else if (mComparator(mHeapArray[rightChildIndex], mHeapArray[leftChildIndex]))  // right child < left child (min heap) or right child > left child (max heap)
            childIndex = rightChildIndex;
        else    
            childIndex = leftChildIndex;

        // parent <= child (min heap) or parent >= child (max heap)
        if (mComparator(mHeapArray[index], mHeapArray[childIndex]) || !mComparator(mHeapArray[index], mHeapArray[childIndex]) && !mComparator(mHeapArray[childIndex], mHeapArray[index]))
            return;

        // else swap parent and child
        T temp = std::move(mHeapArray[index]);
        mHeapArray[index] = std::move(mHeapArray[childIndex]);
        mHeapArray[childIndex] = std::move(temp);

        //using std::swap;
        //swap(mHeapArray[index], mHeapArray[childIndex]);

        index = childIndex;
    }
}

template <typename T, typename F>
void Heap<T,F>::TrickleDownCopy(size_t index)
{
    // save the element
    T temp = std::move(mHeapArray[index]);

    // move the hole down
    while (GetLeftChildIndex(index) < mNumElements)
    {
        size_t leftChildIndex = GetLeftChildIndex(index);
        size_t rightChildIndex = GetRightChildIndex(index);

        size_t childIndex;

        if (rightChildIndex >= mNumElements)
            childIndex = leftChildIndex;
        else if (mComparator(mHeapArray[rightChildIndex], mHeapArray[leftChildIndex]))  // right child < left child
            childIndex = rightChildIndex;
        else    
            childIndex = leftChildIndex;

        // parent <= child (min heap) or parent >= child (max heap)
        if (mComparator(temp, mHeapArray[childIndex]) || !mComparator(temp, mHeapArray[childIndex]) && !mComparator(mHeapArray[childIndex], temp))
            break;

        // else move the hole down
        mHeapArray[index] = std::move(mHeapArray[childIndex]);

        index = childIndex;
    }

    // fill the hole
    mHeapArray[index] = std::move(temp);
}

#endif  // HEAP_H