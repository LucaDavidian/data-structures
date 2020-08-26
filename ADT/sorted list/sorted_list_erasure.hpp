#ifndef SORTED_LIST_H
#define SORTED_LIST_H

#include "../../vector/vector.hpp"
#include "../../../function/function.hpp"
#include <utility>

template <typename T>
bool Less(const T &a, const T &b)
{
    return a < b;
}

template <typename T, template <typename> class Container = Vector>
class SortedList
{
public:
    using Iterator = typename Container<T>::Iterator;
    using ConstIterator = typename Container<T>::ConstIterator;
public: 
    SortedList(const Function<bool(const T&, const T&)> &comparator = &Less<T>) : mComparator(comparator) {}

    bool Empty() const { return mContainer.Empty(); }
    size_t Size() const { return mContainer.Size(); }

    Iterator Begin() { return mContainer.Begin(); }
    ConstIterator Begin() const { return mContainer.Begin(); }
    ConstIterator CBegin() const { return mContainer.CBegin(); }
    Iterator End() { return mContainer.End(); }
    ConstIterator End() const { return mContainer.End(); }
    ConstIterator CEnd() const { return mContainer.CEnd(); }

    template <typename U>
    void Insert(U &&element);

    void Remove(int index) { return mContainer.Remove(index); }
    void RemoveFirst() { mContainer.RemoveFirst(); }
    void RemoveLast() { mContainer.RemoveLast(); }
    Iterator Remove(Iterator iterator) { return mContainer.Remove(iterator); }

    Iterator Find(const T &key);   // binary search

    T &operator[](int index) { return mContainer[index]; }
    const T &operator[](int index) const { return mContainer[index]; }
    T &First(){ return mContainer.First(); }
    const T &First() const { return mContainer.First(); }
    T &Last() { return mContainer.Last(); }
    const T &Last() const { return mContainer.Last(); }
private: 
    Container<T> mContainer;
    Function<bool(const T&, const T&)> mComparator;
};

template <typename T, template <typename> class C>
template <typename U>
void SortedList<T,C>::Insert(U &&element)
{
    Iterator it = mContainer.Begin();

    while (it != mContainer.End())
    {
        if (mComparator(element, *it) || !mComparator(element, *it)&& !mComparator(*it, element))
            break;  // if element to insert <= current element

        ++it;
    }
    
    mContainer.Insert(it, std::forward<U>(element));   
}

// binary search
template <typename T, template <typename> class C>
typename SortedList<T,C>::Iterator SortedList<T,C>::Find(const T &key)
{
    int middleIndex = mContainer.Size();

    while (middleIndex > 0)
    {
        middleIndex /= 2;

        Iterator it = mContainer.AtIndex(middleIndex);

        if (*it == key)
            return it;
    }

    return mContainer.End();    
}

#endif  // SORTED_LIST_H