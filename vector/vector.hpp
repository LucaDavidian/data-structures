#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>
#include <utility>
#include <exception>
#include <initializer_list>
#include <type_traits>

using std::size_t;

class IndexOutOfBoundsException : public std::exception {};

template <typename T>
class Vector;

template <typename T>
void swap(Vector<T> &a, Vector<T> &b)
{
    a.Swap(b);
}

template <typename T>
class Vector
{
public:
    typedef T *Iterator;             // random access iterator
    typedef const T *ConstIterator;  // implicit conversion from Iterator to ConstIterator
public:
    Vector() : mArray(nullptr), mCapacity(0), mNumElements(0) {}
    Vector(size_t size);
    Vector(const Vector &other);
    template <typename U>
    Vector(const Vector<U> &other);
    Vector(Vector &&other);
    template <typename U>
    Vector(Vector<U> &&other);
    Vector(std::initializer_list<T> initList);    // sequence ctor

    ~Vector() { if (mArray) Clear(); }

    Vector &operator=(const Vector &other);
    template <typename U>
    Vector &operator=(const Vector<U> &other);
    Vector &operator=(Vector &&other);
    template <typename U>
    Vector &operator=(Vector<U> &&other);

    void Swap(Vector &other);

    size_t Size() const { return mNumElements; }
    size_t Capacity() const { return mCapacity; }
    bool Empty() const { return mNumElements == 0; }

    T *Data() { return const_cast<T*>(static_cast<const Vector&>(*this).Data()); }
    const T *Data() const { return mArray; }

    void Resize(size_t size, const T &element = T());
    void Reserve(size_t size);   // grow - TODO: shrink
    void Clear(); 

    Iterator Begin() { return &mArray[0]; }
    ConstIterator Begin() const { return &mArray[0]; }   // implicit conversion from T* to const T*
    ConstIterator CBegin() const { return Begin(); }
    Iterator End() { return &mArray[mNumElements]; }     // return past the end pointer
    ConstIterator End() const { return &mArray[mNumElements]; }
    ConstIterator CEnd() const { return End(); }

    template <typename U>
    void Insert(int index, U &&element);
    template <typename U>
    void InsertFirst(U &&element) { Insert(0, std::forward<U>(element)); }
    template <typename U>
    void InsertLast(U &&element) { Insert(mNumElements, std::forward<U>(element)); }
    template <typename U>
    Iterator Insert(Iterator pos, U &&element);             // insert element before pos and return iterator after inserted element
    template <typename Iter>
    Iterator Insert(Iterator pos, Iter begin, Iter end);    // insert iterator range before pos and return iterator after last inserted element
    
    template <typename... Args, typename U = T, typename = typename std::enable_if<std::is_aggregate<U>::value>::type>
    void Emplace(int index, Args&&... args);
    template <typename... Args, typename U = T, typename = typename std::enable_if<!std::is_aggregate<U>::value>::type, typename = void>
    void Emplace(int index, Args&&... args);
    template <typename... Args>
    void EmplaceFirst(Args&&... args) { Emplace(0, std::forward<Args>(args)...); }
    template <typename... Args>
    void EmplaceLast(Args&&... args) { Emplace(mNumElements, std::forward<Args>(args)...); }
    template <typename... Args, typename U = T, typename = typename std::enable_if<std::is_aggregate<U>::value>::type>
    Iterator Emplace(Iterator pos, Args&&... args);
    template <typename... Args, typename U = T, typename = typename std::enable_if<!std::is_aggregate<U>::value>::type, typename = void>
    Iterator Emplace(Iterator pos, Args&&... args);

    void Remove(int index);
    void RemoveFirst() { Remove(0); }
    void RemoveLast() { Remove(mNumElements - 1); }
    Iterator Remove(Iterator pos);
    Iterator Remove(Iterator begin, Iterator end);

    T &operator[](int index) { return const_cast<T&>(static_cast<Vector const&>(*this)[index]); }
    const T &operator[](int index) const { return mArray[index]; }
    T &First(){ return const_cast<T&>(static_cast<Vector const&>(*this).First()); }
    const T &First() const { return mArray[0]; }
    T &Last() { return const_cast<T&>(static_cast<Vector const&>(*this).Last()); }
    const T &Last() const { return mArray[mNumElements - 1]; }

    Iterator AtIndex(size_t index) const { return &mArray[index]; }
    int IndexOf(ConstIterator iterator) { return iterator - mArray; }

    Iterator Find(const T &key) { return const_cast<Iterator>(const_cast<Vector<T> const&>(*this).Find(key)); }
    ConstIterator Find(const T &key) const;
private:
    T *mArray;
    size_t mCapacity;
    size_t mNumElements;
};

// begin and end functions (to use in range-for loop)
template <typename T>
typename Vector<T>::Iterator begin(Vector<T> &vector)
{
    return vector.Begin();
}

template <typename T>
typename Vector<T>::Iterator end(Vector<T> &vector)
{
    return vector.End();
}

template <typename T>
typename Vector<T>::ConstIterator begin(const Vector<T> &vector)
{
    return vector.Begin();
}

template <typename T>
typename Vector<T>::ConstIterator end(const Vector<T> &vector)
{
    return vector.End();
}

template <typename T>
Vector<T>::Vector(size_t size)
{
    // allocate untyped memory for array (operator new)
    void *rawMemory = operator new(size * sizeof(T));

    // construct elements in-place (placement-new)
    for (size_t i = 0; i < size; i++)
        new(&static_cast<T*>(rawMemory)[i]) T;  // T default constructible

    // set array, capacity and element count
    mArray = static_cast<T*>(rawMemory);
    mCapacity = size;
    mNumElements = size;
}

template <typename T>
Vector<T>::Vector(const Vector &other)
{
    // allocate untyped memory for array (operator new)
    void *rawMemory = operator new(other.mCapacity * sizeof(T));

    // copy elements (placement-new)
    for (size_t i = 0; i < other.mNumElements; i++)
        new(&static_cast<T*>(rawMemory)[i]) T(other.mArray[i]);

    // set array, capacity and element count
    mArray = static_cast<T*>(rawMemory);
    mCapacity = other.mCapacity;
    mNumElements = other.mNumElements;
}

template <typename T>
template <typename U>
Vector<T>::Vector(const Vector<U> &other)
{
    // allocate untyped memory for array (operator new)
    void *rawMemory = operator new(other.mCapacity * sizeof(T));

    // copy elements (placement-new)
    for (size_t i = 0; i < other.mNumElements; i++)
        new(&static_cast<T*>(rawMemory)[i]) T(other.mArray[i]);

    // set array, capacity and element count
    mArray = static_cast<T*>(rawMemory);
    mCapacity = other.mCapacity;
    mNumElements = other.mNumElements;
}

template <typename T>  // "steal" moved from vector resources
Vector<T>::Vector(Vector &&other) : mArray(other.mArray), mCapacity(other.mCapacity), mNumElements(other.mNumElements)
{
    // set moved from array to null
    other.mArray = nullptr;    
}

template <typename T>
template <typename U>
Vector<T>::Vector(Vector<U> &&other) 
{
    // allocate untyped memory for array (operator new)
    void  *rawMemory = operator new(other.mCapacity * sizeof(T));

    // move elements (placement-new)
    for (size_t i = 0; i < other.mNumElements; i++)
        new(&static_cast<T*>(rawMemory)[i]) T(std::move(other.mArray[i]));  // T constructible from U

    // set array, capacity and element count
    mArray = static_cast<T*>(rawMemory);
    mCapacity = other.Capacity;
    mNumElements = other.mNumElements;

    // set moved from array to null
    other.mArray = nullptr;
}

template <typename T>
Vector<T>::Vector(std::initializer_list<T> initList) : mArray(nullptr), mCapacity(0), mNumElements(0)
{
    Reserve(initList.size());

    for (auto &element : initList)
        InsertLast(element);
}

template <typename T>
void Vector<T>::Clear()
{
    // destroy elements
    for (size_t i = 0; i < mNumElements; i++)
        mArray[i].~T();
    
    mNumElements = 0;

    // free allocated memory
    operator delete(mArray);

    mArray = nullptr;
    mCapacity = 0;
}

template <typename T>
void Vector<T>::Resize(size_t size, const T &element)
{
    if (size == mNumElements)
        return;
    else if (size > mNumElements)
    {
        size_t diff = size - mNumElements;

        while (diff--)
            InsertLast(element);
    }
    else  // size < mNumElements
        while (mNumElements > size)
            RemoveLast();
}

template <typename T>
void Vector<T>::Reserve(size_t size)
{
    if (mCapacity >= size)
        return;

    // allocate new array (sizeof(T) * size bytes)
    void *rawMem = operator new(size * sizeof(T));

    // move elements to new array
    for (size_t i = 0; i < mNumElements; i++)
        new(&static_cast<T*>(rawMem)[i]) T(std::move(mArray[i]));  // placement-new + std::move

    // destroy old elements
    for (size_t i = 0; i < mNumElements; i++)
        mArray[i].~T();

    // free old array
    operator delete(mArray);

    // set new array
    mArray = static_cast<T*>(rawMem);

    // set new capacity
    mCapacity = size;
}

template <typename T>
void Vector<T>::Swap(Vector &other)
{
    using std::swap;

    // swap pointer, capacity and element count
    swap(mArray, other.mArray);
    swap(mCapacity, other.mCapacity);
    swap(mNumElements, other.mNumElements);

    // // swap pointer, capacity and element count
    // T *tempArray = mArray;
    // mArray = other.mArray;
    // other.mArray = tempArray;

    // size_t tempCapacity = mCapacity;
    // mCapacity = other.mCapacity;
    // other.mCapacity = tempCapacity;

    // size_t tempNumElements = mNumElements;
    // mNumElements = other.mNumElements;
    // other.mNumElements = tempNumElements;
}

template <typename T>
Vector<T> &Vector<T>::operator=(const Vector &other)
{
    // copy and swap
    Vector temp(other);  // copy
    Swap(temp);          // swap

    return *this;
}

template <typename T>
template <typename U>
Vector<T> &Vector<T>::operator=(const Vector<U> &other)
{
    // copy and swap
    Vector<T> temp(other);  // generalized copy constructor
    Swap(temp);             // swap

    return *this;
}

template <typename T>
Vector<T> &Vector<T>::operator=(Vector &&other)
{
    Swap(other);

    return *this;
}

template <typename T>
template <typename U>
Vector<T> &Vector<T>::operator=(Vector<U> &&other)
{
    Vector<T> temp(std::move(other));  // generalized move constructor
    Swap(temp);                        // swap

    return *this;
}

template <typename T>
template <typename U>
void Vector<T>::Insert(int index, U &&element)   
{
    if (index < 0 || index > mNumElements)  
        throw IndexOutOfBoundsException();

    if (mNumElements >= mCapacity)
        Reserve(mCapacity == 0 ? 1 : mCapacity * 2);

    if (index == mNumElements)  // if insert last or vector empty
        new(&mArray[index]) T(std::forward<U>(element));   // copy/move-construct element (placement-new)
    else 
    {
        // move-construct new last element 
        new(&mArray[mNumElements]) T(std::move(mArray[mNumElements - 1]));

        // shift elements up one place
        for (size_t i = mNumElements - 1; i > index; i--)
            mArray[i] = std::move(mArray[i - 1]);

        // insert (copy/move-assing) element at index 
        mArray[index] = std::forward<U>(element);
    }

    mNumElements++;
}

template <typename T>
template <typename U>
typename Vector<T>::Iterator Vector<T>::Insert(Iterator pos, U &&element)
{
    // size_t index = IndexOf(iterator);
    // Insert(index, std::forward<U>(element));
    // return AtIndex(index);

    if (mNumElements >= mCapacity)   
    {
        size_t index = IndexOf(pos);  // save iterator
        Reserve(mCapacity == 0 ? 1 : mCapacity * 2);
        pos = AtIndex(index);  // restore iterator
    }

    if (pos == End())  // if insert last or vector empty
        new(pos) T(std::forward<U>(element));   // copy/move-construct element (placement-new)
    else
    {
        // move-construct new last element 
        Iterator endIt = End();
        new(endIt) T(std::move(*(endIt - 1)));  

        // shift elements up one place
        for (Iterator it = endIt - 1; it > pos; --it)
            *it = std::move(*(it - 1));

        // insert (copy/move-assing) element at index 
        *pos = std::forward<U>(element);
    }

    mNumElements++;

    return ++pos;
}

template <typename T>
template <typename Iter>
typename Vector<T>::Iterator Vector<T>::Insert(Iterator pos, Iter begin, Iter end)
{
    if (begin == end)
       return pos;

    int numElementsToInsert = end - begin;
    int numElementsToShift = End() - pos;

    if (mNumElements + numElementsToInsert >= mCapacity)
    {
        size_t index = IndexOf(pos);
        Reserve(mNumElements + numElementsToInsert);
        pos = AtIndex(index);
    }

    Iterator to = End() + numElementsToInsert - 1;
    Iterator from = End() - 1;

    for (int i = 0, j = numElementsToInsert; i < numElementsToShift; i++)  // shift elements to the end of the array
    {
        if (j > 0)
        {
            new(to--) T(std::move(*from--));
            j--;
        }
        else
            *to = std::move(*from--);      
    }

    while (begin != end)
        if (numElementsToShift > 0)
        {
            *pos++ = *begin++;
            numElementsToShift--;
        }
        else
            new(pos++) T(*begin++);

    mNumElements += numElementsToInsert;

    return pos;
}

template <typename T>
template <typename... Args, typename, typename>
void Vector<T>::Emplace(int index, Args&&... args) 
{ 
    Insert(index, T{std::forward<Args>(args)...}); 
}

template <typename T>
template <typename... Args, typename, typename, typename>
void Vector<T>::Emplace(int index, Args&&... args) 
{ 
    Insert(index, T(std::forward<Args>(args)...)); 
}

template <typename T>
template <typename... Args, typename, typename>
typename Vector<T>::Iterator Vector<T>::Emplace(Iterator pos, Args&&... args)
{
    return Insert(pos, T{std::forward<Args>(args)...}); 
}

template <typename T>
template <typename... Args, typename U, typename, typename>
typename Vector<T>::Iterator Vector<T>::Emplace(Iterator pos, Args&&... args)
{
    return Insert(pos, T(std::forward<Args>(args)...)); 
}

template <typename T>
void Vector<T>::Remove(int index)
{
    if (index < 0 || index >= mNumElements) 
        throw IndexOutOfBoundsException();

    // shift elements down one place
    for (size_t i = index; i < mNumElements - 1; i++)
        mArray[i] = std::move(mArray[i + 1]);

    // destroy last element
    mArray[mNumElements - 1].~T();

    mNumElements--;
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::Remove(Iterator pos)
{
    // shift elements down one place
    Iterator it;
    for (it = pos; it < End() - 1; ++it)
        *it = std::move(*(it + 1));

    // destroy last element
    it->~T();

    mNumElements--;

    return pos;
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::Remove(Iterator begin, Iterator end)
{
    Iterator it1 = begin;
	Iterator it2 = end;

	while (it1 != begin && it2 != End())
		*it1++ = std::move(*it2++);

	while (it1 != End())
		it1++->~T();

	mNumElements -= end - begin;
	
	return end;
}

template <typename T>
typename Vector<T>::ConstIterator Vector<T>::Find(const T &key) const
{
    for (std::size_t i = 0; i < mNumElements; i++)
        if (key == mArray[i])
            return ConstIterator(&mArray[i]);

    return End();
}

#endif  // VECTOR_H