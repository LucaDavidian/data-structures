#ifndef MAP_H
#define MAP_H

/**** map - hash table implementation (no duplicates) ****/

#include "../../../vector/vector.hpp"
#include "../../../linked list/double_ended_doubly_linked_list.hpp"
#include "../../../../function/function.hpp"
#include <utility>
#include <exception>
#include <cstddef>
#include <string>

using std::size_t;

class ElementNotPresentException : std::exception
{
};

template <typename T>
size_t Hash(const T &key, size_t);

template <>
size_t Hash<std::string>(const std::string &s, size_t tableSize)
{
    return s.size() % tableSize;
}

template <typename K, typename V>
class Entry
{
public:
    Entry(const K &key, const V &value) : mKey(key), mValue(value) {}
    K &Key() { return mKey; }
    V &Value() { return mValue; }
    operator Entry<const K,V>() { return Entry<const K,V>(mKey, mValue); }
private:
    K mKey;
    V mValue;
};

template <typename K, typename V>
class Map
{
private:
    typedef Entry<const K,V> Entry;
    typedef DoublyLinkedList<Entry> Bucket;
    typedef Vector<Bucket> BucketArray;
    using BucketArrayIterator = typename BucketArray::Iterator;
    using BucketIterator = typename Bucket::Iterator;
    typedef Function<size_t(const K &, size_t)> HashFunction;
public:
    class Iterator
    {
    friend class Map;
    public:
        Iterator(BucketArray *bucketArray, BucketArrayIterator bucketArrayIterator, BucketIterator bucketIterator);
        // overloaded operators
        Entry &operator*() { return *mBucketIterator; }
        Entry *operator->() { return &operator*(); }
        Iterator operator++();
        Iterator operator++(int) { Iterator temp(*this); ++*this; return temp; }
        bool operator==(const Iterator &other);
        bool operator!=(const Iterator &other) { return !operator==(other); }    
    private:
        BucketArray *mBucketArray;
        BucketArrayIterator mBucketArrayIterator;
        BucketIterator mBucketIterator;
    };
public:
    Map(size_t size, const HashFunction & = &Hash<K>);

    bool Empty() const { return mNumElements == 0; }
    size_t Size() const { return mNumElements; }

    template <typename FwdK, typename FwdV>
    Iterator Insert(FwdK &&, FwdV &&);
    template <typename E>
    Iterator Insert(E &&);

    Iterator Remove(const K &);
    Iterator Remove(const Iterator &);

    Iterator Find(const K &);

    Iterator Begin();
    Iterator End();
private:
    BucketArray mBucketArray;
    size_t mSize;
    size_t mNumElements;

    HashFunction mHashFunction;
};

/**** map's iterator implementation ****/

template <typename K, typename V>
Map<K,V>::Iterator::Iterator(BucketArray *bucketArray, BucketArrayIterator bucketArrayIterator, BucketIterator bucketIterator)
    : mBucketArray(bucketArray), mBucketArrayIterator(bucketArrayIterator), mBucketIterator(bucketIterator)
{
}

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::Iterator::operator++()
{
    ++mBucketIterator;

    if (mBucketIterator == mBucketArrayIterator->End())
    {
        do
        {
            ++mBucketArrayIterator;
        } while (mBucketArrayIterator != mBucketArray->End() && mBucketArrayIterator->Empty());

        if (mBucketArrayIterator != mBucketArray->End())
            mBucketIterator = mBucketArrayIterator->Begin();        
    }

    return *this;
}

template <typename K, typename V>
bool Map<K,V>::Iterator::operator==(const Iterator &other)
{
    if (mBucketArray != other.mBucketArray || mBucketArrayIterator != other.mBucketArrayIterator)
        return false;

    if (mBucketArrayIterator == mBucketArray->End())
        return true;

    return mBucketIterator == other.mBucketIterator;
}

/**** map implementation ****/

template <typename K, typename V>
Map<K,V>::Map(size_t size, const HashFunction &hashFunction)
    : mNumElements(0), mSize(size), mHashFunction(hashFunction)
{
    for (size_t i = 0; i < mSize; i++)
        mBucketArray.InsertLast(Bucket());
}

template <typename K, typename V>
template <typename FwdK, typename FwdV>
typename Map<K,V>::Iterator Map<K,V>::Insert(FwdK &&key, FwdV &&value)
{
    Iterator it = Find(key);

    mNumElements++;

    if (it != End())
    {
        it->Value() = std::forward<FwdV>(value);

        return it;
    }
    else
    {
        size_t index = mHashFunction(key, mSize);

        BucketArrayIterator bucketArrayIterator = mBucketArray.Begin() + index;
        BucketIterator bucketIterator = bucketArrayIterator->Insert(bucketArrayIterator->End(), Entry(std::forward<FwdK>(key), std::forward<FwdV>(value)));

        return Iterator(&mBucketArray, bucketArrayIterator, bucketIterator);
    }
}

template <typename K, typename V>
template <typename E>
typename Map<K,V>::Iterator Map<K,V>::Insert(E &&entry)
{
    Iterator it = Find(entry.Key());

    mNumElements++;

    if (it != End())
    {
        //it->Value() = std::forward< E >(entry.Value());  ???
        it->Value() = entry.Value();

        return it;
    } 
    else
    {
        size_t index = mHashFunction(entry.Key(), mSize);

        BucketArrayIterator bucketArrayIterator = mBucketArray.Begin() + index;
        BucketIterator bucketIterator = bucketArrayIterator->Insert(bucketArrayIterator->End(), std::forward<E>(entry));

        return Iterator(&mBucketArray, bucketArrayIterator, bucketIterator);
    }
}

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::Remove(const K &key)
{
    size_t index = mHashFunction(key, mSize);

    BucketArrayIterator bucketArrayIterator = mBucketArray.Begin() + index;
    BucketIterator bucketIterator = bucketArrayIterator->Begin();

    while (bucketIterator != bucketArrayIterator->End())
    {
        if (key == bucketIterator->Key())
        {
            bucketArrayIterator->IT_Remove(bucketIterator);

            mNumElements--;

            return Iterator(&mBucketArray, bucketArrayIterator, bucketIterator);
        }
        
        ++bucketIterator;
    }

    throw ElementNotPresentException();
}

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::Remove(const Iterator &iterator)
{
    BucketIterator bucketIterator = iterator.mBucketArrayIterator->Remove(iterator.mBucketIterator);

    mNumElements--;

    return Iterator(iterator.mBucketArray, iterator.mBucketArrayIterator, bucketIterator);
}

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::Find(const K &key)
{
    size_t index = mHashFunction(key, mSize);

    BucketArrayIterator bucketArrayIterator = mBucketArray.Begin() + index;
    BucketIterator bucketIterator = bucketArrayIterator->Begin();

    while (bucketIterator != bucketArrayIterator->End())
    {
        if (key == bucketIterator->Key())
            return Iterator(&mBucketArray, bucketArrayIterator, bucketIterator);

        ++bucketIterator;
    }

    return End();
}

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::Begin()
{
    BucketArrayIterator bucketArrayIterator = mBucketArray.Begin();
    BucketIterator bucketIterator = (mBucketArray.End() - 1)->End();

    while (bucketArrayIterator != mBucketArray.End())
    {
        if (!bucketArrayIterator->Empty())
        {
            bucketIterator = bucketArrayIterator->Begin();
            break;
        }
     
        ++bucketArrayIterator;
    }      

    return Iterator(&mBucketArray, bucketArrayIterator, bucketIterator);
}

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::End()
{
    BucketIterator last = (mBucketArray.End() - 1)->End();
    return Iterator(&mBucketArray, mBucketArray.End(), last);
}

#endif  // MAP_H