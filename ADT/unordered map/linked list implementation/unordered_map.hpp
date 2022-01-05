/**** map - doubly linked list implementation (no duplicates) ****/

#ifndef MAP_H
#define MAP_H

#include "../../../linked list/double_ended_doubly_linked_list.hpp"
#include <exception>

template <typename K, typename V>
class Map;

template <typename K, typename V>
class Entry
{
template <typename, typename> friend class Map;     
public:
    Entry(const K &key, const V &value) : mKey(key), mValue(value) {}
    ~Entry() = default;
    K &Key() { return mKey; }
    V &Value() { return mValue; }
private:
    K mKey;
    V mValue;
};

template <typename K, typename V>
class Map
{
public:
    using Entry = Entry<const K,V>;
    typedef typename DoublyLinkedList<Entry>::Iterator Iterator;  
public:
    bool Empty() { return mDataStruct.Empty(); }
    std::size_t Size() { return mDataStruct.Size(); }

    Iterator Find(const K&);
    
    template <typename FWDK, typename FWDV>
    Iterator Insert(FWDK&&, FWDV&&);

    Iterator Remove(const K&);

    Iterator Begin() { return mDataStruct.Begin(); }
    Iterator End() { return mDataStruct.End(); }
private:
    DoublyLinkedList<Entry> mDataStruct;
};

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::Find(const K &key)
{
    typename DoublyLinkedList<Entry>::Iterator it = mDataStruct.Begin();

    while (it != mDataStruct.End())
    {
        if (it->Key() == key)
            return it;

        ++it;
    }

    return it;
}

template <typename K, typename V>
template <typename FWDK, typename FWDV>
typename Map<K,V>::Iterator Map<K,V>::Insert(FWDK &&key, FWDV &&value)
{
    Iterator it = Find(key);

    if (it != End())    // key present
    {
        it->mValue = std::forward<FWDV>(value);   // update value

        return it;
    }
    else  // not present
    {
        mDataStruct.InsertLast(Entry(std::forward<FWDK>(key), std::forward<FWDV>(value)));  // add entry
        
        return --End();
    }
}

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::Remove(const K &key)
{
    Iterator it = Find(key);

    if (it == End())
        throw std::exception();

    return mDataStruct.Remove(it);
}

#endif  // MAP_H