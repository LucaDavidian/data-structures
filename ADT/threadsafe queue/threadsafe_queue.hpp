#ifndef THREADSAFE_QUEUE_H
#define THREADSAFE_QUEUE_H

#include <mutex>
#include <cstddef>

#include "../../linked list/double_ended_singly_linked_list.hpp"

using std::size_t;

// with double ended linked list all operations are implemented in O(1) constant time
template <typename T, template <typename> class S = DE_SinglyLinkedList>  
class ThreadsafeQueue
{
public:
    ThreadsafeQueue() = default;
    ~ThreadsafeQueue() { Clear(); }

    bool Empty() const { std::lock_guard<std::mutex>(mMutex); return mContainer.Empty(); }
    size_t Size() const { std::lock_guard<std::mutex>(mMutex); return mContainer.Size(); }

    void Clear() { std::lock_guard<std::mutex>(mMutex); mContainer.Clear(); }

    template <typename U>
    void EnQueue(U &&element) { std::lock_guard<std::mutex>(mMutex); mContainer.InsertLast(std::forward<U>(element)); }
    void DeQueue() { std::lock_guard<std::mutex>(mMutex); mContainer.RemoveFirst(); }

    T Front() const { std::lock_guard<std::mutex>(mMutex);  mContainer.First(); }
    T Back() const { std::lock_guard<std::mutex>(mMutex); return mContainer.Last(); }
private:
    std::mutex mMutex;
    S<T> mContainer;
};

#endif  // THREADSAFE_QUEUE_H