#ifndef BUBBLE_SORT_H
#define BUBBLE_SORT_H

template <template <typename> class S, typename T>
void BubbleSort(S<T> &sequence)
{
    for (size_t i = sequence.Size() - 1; i > 0; i--)
        for (size_t j = 0; j < i; j++)
        {
            if (*sequence.AtIndex(j) > *sequence.AtIndex(j + 1))
            {
                T temp = *sequence.AtIndex(j);
                *sequence.AtIndex(j) = *sequence.AtIndex(j + 1);
                *sequence.AtIndex(j + 1) = temp;
            }
        }
}

template <typename T, template <class> class S>
void IT_BubbleSort(S<T> &vec)
{
    typename S<T>::Iterator it = vec.End();

    while (it != vec.Begin())
    {
        typename S<T>::Iterator prec = vec.Begin(), succ = prec;
        ++succ;

        while (succ != it)
        {
            if (*prec > *succ)
            {
                T temp = *prec;
                *prec = *succ;
                *succ = temp;
            } 
            ++prec;
            ++succ;
        } 
        --it;  
    }
}

#endif  // BUBBLE_SORT_H