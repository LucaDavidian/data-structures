#ifndef HASH_TABLE_H
#define HASH_TABLE_H

/**** hash table - separate chaining implementation ****/

#include <cstddef>
#include <exception>

#include "../../type erasure/type erasure - function/function.hpp"
#include "../vector/vector.hpp"
#include "../list/double_ended_doubly_linked_list.hpp"

using std::size_t;

class ElementNotPresentException : public std::exception {};

/**** generic Hash function ****/
template <typename T>
size_t Hash(const T &, size_t);

#include <string>

template <>
size_t Hash<std::string>(const std::string &s, size_t tableSize)
{
    return s.size() % tableSize;
}

template <typename T>
class HashTable
{
private:
    typedef DoublyLinkedList<T> Bucket;
    typedef Vector<Bucket> BucketArray;
    using BucketArrayIterator = typename BucketArray::Iterator;
    using BucketIterator = typename Bucket::Iterator;
public:
    class Iterator
    {
    friend class HashTable; // friend class HashTable<T>
    public:
        Iterator(BucketArray *bucketArray, const BucketArrayIterator &bucketArrayIterator, const BucketIterator &bucketIterator);
       
        T &operator*() { return *mBucketIterator; }
        T *operator->() { return &*mBucketIterator; }
        Iterator &operator++();
        Iterator operator++(int);
        bool operator==(const Iterator &other);
        bool operator!=(const Iterator &other) { return !operator==(other); }
    private:
        BucketArray *mBucketArray;
        BucketArrayIterator mBucketArrayIterator;
        BucketIterator mBucketIterator;
    };
public:
    HashTable(size_t, const Function<size_t(const T &, size_t)> & = Hash<T>);

    float GetLoadFactor() const { return (float)mNumElements / (float)mSize; }
    bool Empty() const { return mNumElements == 0; }
    size_t Size() const { return mNumElements; }

    template <typename U>
    Iterator Insert(U &&);

    Iterator Remove(const T &);
    Iterator Remove(const Iterator &);

    Iterator Find(const T &);

    Iterator Begin();
    Iterator End();
private:
    BucketArray mBucketArray;  // Vector<DoublyLinkedList<T>>
    size_t mSize;
    size_t mNumElements;

    Function<size_t(const T&, size_t)> mHashFunction;
};

/**** HashTable's Iterator member definitions ****/

template <typename T>
HashTable<T>::Iterator::Iterator(BucketArray *bucketArray, const BucketArrayIterator &bucketArrayIterator, const BucketIterator &bucketIterator)
    : mBucketArray(bucketArray), mBucketArrayIterator(bucketArrayIterator), mBucketIterator(bucketIterator)
{
}

template <typename T>
typename HashTable<T>::Iterator &HashTable<T>::Iterator::operator++()
{
    ++mBucketIterator;

    if (mBucketIterator == mBucketArrayIterator->End())
    {
        do
            ++mBucketArrayIterator;
        while (mBucketArrayIterator != mBucketArray->End() && mBucketArrayIterator->Empty());            
    
        if (mBucketArrayIterator != mBucketArray->End())
            mBucketIterator = mBucketArrayIterator->Begin();
    }

    return *this;
}

template <typename T>
typename HashTable<T>::Iterator HashTable<T>::Iterator::operator++(int)
{
    Iterator temp = *this;
    ++*this;
    
    return temp;
}

template <typename T>
bool HashTable<T>::Iterator::operator==(const Iterator &other)
{
    if (mBucketArray != other.mBucketArray || mBucketArrayIterator != other.mBucketArrayIterator)
        return false;

    if (mBucketArrayIterator == mBucketArray->End())
        return true;

    return mBucketIterator == other.mBucketIterator;
}

/**** HashTable's member definitions ****/

template <typename T>
HashTable<T>::HashTable(size_t size, const Function<size_t(const T&, size_t)> &hashFunction) 
    : mSize(size), mNumElements(0), mHashFunction(hashFunction)
{
    for (size_t i = 0; i < mSize; i++)
        mBucketArray.InsertLast(DoublyLinkedList<T>());
}

template <typename T>
template <typename U>
typename HashTable<T>::Iterator HashTable<T>::Insert(U &&element)
{
    size_t index = mHashFunction(element, mSize);

    BucketArrayIterator bucketArrayIterator = &mBucketArray[index];
    BucketIterator bucketIterator = bucketArrayIterator->IT_InsertLast(std::forward<U>(element));

    mNumElements++;

    return Iterator(&mBucketArray, bucketArrayIterator, bucketIterator);
}

template <typename T>
typename HashTable<T>::Iterator HashTable<T>::Remove(const T &key)
{
    size_t index = mHashFunction(key, mSize);

    BucketArrayIterator bucketArrayIterator = &mBucketArray[index];
    BucketIterator bucketIterator = bucketArrayIterator->Find(key);

    if (bucketIterator == bucketArrayIterator->End())
        throw ElementNotPresentException();

    bucketIterator = bucketArrayIterator->IT_Remove(bucketIterator);

    mNumElements--;

    return Iterator(&mBucketArray, bucketArrayIterator, bucketIterator);
}

template <typename T>
typename HashTable<T>::Iterator HashTable<T>::Remove(const Iterator &iterator)
{
    BucketArrayIterator bucketArrayIterator = iterator.mBucketArrayIterator;
    BucketIterator bucketIterator = iterator.mBucketIterator;

    bucketIterator = bucketArrayIterator->IT_Remove(bucketIterator);
    
    mNumElements--;

    return Iterator(&mBucketArray, bucketArrayIterator, bucketIterator);
}

template <typename T>
typename HashTable<T>::Iterator HashTable<T>::Find(const T &key)
{
    size_t index = mHashFunction(key, mSize);

    BucketArrayIterator bucketArrayIterator = &mBucketArray[index];
    BucketIterator bucketIterator = bucketArrayIterator->Find(key);

    if (bucketIterator == bucketArrayIterator->End())
        return End();

    return Iterator(&mBucketArray, bucketArrayIterator, bucketIterator);
}

template <typename T>
typename HashTable<T>::Iterator HashTable<T>::Begin()
{
    BucketArrayIterator bucketArrayIterator = mBucketArray.Begin();

    while (bucketArrayIterator != mBucketArray.End())
    {
        if (bucketArrayIterator->Empty())
            ++bucketArrayIterator;
        else
            return Iterator(&mBucketArray, bucketArrayIterator, bucketArrayIterator->Begin());
    }

    return End();  // hash table is empty - return off-the-end iterator
}

template <typename T>
typename HashTable<T>::Iterator HashTable<T>::End()
{
    return Iterator(&mBucketArray, mBucketArray.End(), BucketIterator(nullptr, nullptr));
}   

#endif  // HASH_TABLE_H