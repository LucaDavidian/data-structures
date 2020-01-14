#ifndef STACK_H
#define STACK_H

#include <cstddef>

#include "../../vector/vector.hpp"

using std::size_t;

template <typename T, template <typename> class C = Vector>
class Stack
{
public:
    Stack() = default;
    ~Stack() = default;

    bool Empty() const { return mContainer.Empty(); }
    size_t Size() const { return mContainer.Size(); }

    template <typename U>
    void Push(U &&element) { mContainer.InsertLast(std::forward<U>(element)); }
    void Pop() { mContainer.RemoveLast(); }
    T &Top() { return const_cast<T&>(static_cast<const Stack&>(*this).Top()); }  
    const T &Top() const { return mContainer.Last(); }  
private:
    C<T> mContainer;
};

#endif  // STACK_H