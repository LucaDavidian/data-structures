#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#ifdef TYPE_ERASURE_HEAP_QUEUE   // heap implementation (type erased comparator)

    #include "../../heap/heap_vector_erasure.hpp"
    #include "../../../function/function.hpp"

    template <typename T>
    class PriorityQueue
    {
    public:
        PriorityQueue(const Function<bool(const T&, const T&)> &comparator = Less<T>) : mHeap(comparator) {}

        bool Empty() const { return mHeap.Empty(); }
        size_t Size() const { return mHeap.Size(); }

        template <typename U>
        void Insert(U &&element)  { mHeap.Insert(std::forward<U>(element)); }

        void Remove() { mHeap.Remove(); }

        void Remove(const T &element) { mHeap.Remove(element); }

        bool Find(T const &element) const { return mHeap.has(element); }

        const T &Peek() const { return mHeap.Peek(); }
    private:
        Heap<T> mHeap;
    };

#elif defined TYPE_PARAM_HEAP_QUEUE   // heap implementation (template type parameter comparator)

    #include "../../heap/heap_vector_param.hpp"

    template <typename T, typename F = decltype(&Less<T>)>
    class PriorityQueue
    {
    public:
        PriorityQueue(const F &comparator = Less<T>) : mHeap(comparator) {}

        bool Empty() const { return mHeap.Empty(); }
        size_t Size() const { return mHeap.Size(); }

        template <typename U>
        void Insert(U &&element)  { mHeap.Insert(std::forward<U>(element)); }

        void Remove() { mHeap.Remove(); }

        void Remove(const T &element) { mHeap.Remove(element); }

        bool Find(const T &element) const { return mHeap.Has(element); }

        const T &Peek() const { return mHeap.Peek(); }
    private:
        Heap<T, F> mHeap;
    };

#elif defined TYPE_ERASURE_VECTOR_QUEUE   // sorted vector implementation (type erased comparator)

    #include "../../vector/vector.hpp"
    #include "../../../function/function.hpp"

    template <typename T>  // ascendent order by default (last element is head of queue)
    bool Less(const T &a, const T &b)
    {
        return a < b;
    }

    template <typename T>
    class PriorityQueue
    {
    public:
        PriorityQueue(const Function<bool(const T&, const T&)> &comparator = Less<T>) : mComparator(comparator) {}

        size_t Size() const { return mVector.Size(); }
        bool Empty() const { return mVector.Empty(); }

        template <typename U>
        void Insert(U &&element);

        void Remove() { mVector.RemoveLast(); }

        void Remove(const T &element) { mVector.Remove(mVector.Find(element)); }

        bool Find(const T &element) const { return mVector.Find(element) != mVector.End(); }

        const T &Peek() const { return mVector.Last(); }
    private:
        Vector<T> mVector;
        Function<bool(const T&, const T&)> mComparator;
    };

    template <typename T>
    template <typename U>
    void PriorityQueue<T>::Insert(U &&element)
    {
        if (Empty())
            mVector.Insert(0, std::forward<U>(element));
        else
        {
            size_t i;
            for (i = 0; i < mVector.Size(); i++)
                if (mComparator(mVector[i], element) || !mComparator(element, mVector[i]) && !mComparator(mVector[i], element))
                {
                    mVector.Insert(i, std::forward<U>(element));
                    break;
                }

            if (i == mVector.Size())
                mVector.InsertLast(std::forward<U>(element));
        }
    }

#elif defined TYPE_PARAM_VECTOR_QUEUE   // sorted vector implementation (template type parameter comparator)

    #include "../../vector/vector.hpp"

    template <typename T>  // ascendent order by default (last element is head of queue)
    bool Less(const T &a, const T &b)
    {
        return a < b;
    }

    template <typename T, typename F = decltype(&Less<T>)>
    class PriorityQueue
    {
    public:
        PriorityQueue(const F &comparator = &Less<T>) : mComparator(comparator) {}

        size_t Size() const { return mVector.Size(); }
        bool Empty() const { return mVector.Empty(); }

        template <typename U>
        void Insert(U &&element);

        void Remove() { mVector.RemoveLast(); }

        void Remove(const T &element) { mVector.Remove(mVector.Find(element)); }

        bool Find(const T &element) const { return mVector.Find(element) != mVector.End(); }

        const T &Peek() const { return mVector.Last(); }
    private:
        Vector<T> mVector;
        F mComparator;
    };

    template <typename T, typename F>
    template <typename U>
    void PriorityQueue<T,F>::Insert(U &&element)
    {
        if (Empty())
            mVector.Insert(0, std::forward<U>(element));
        else
        {
            size_t i;
            for (i = 0; i < mVector.Size(); i++)
                if (mComparator(mVector[i], element) || !mComparator(element, mVector[i]) && !mComparator(mVector[i], element))
                {
                    mVector.Insert(i, std::forward<U>(element));
                    break;
                }

            if (i == mVector.Size())
                mVector.InsertLast(std::forward<U>(element));
        }
    }

#endif 

#endif  // PRIORITY_QUEUE_H