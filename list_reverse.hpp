template <typename List>
void ListReverse(List &list)
{
    List temp;

    typename List::Iterator it = list.Begin();

    while (it != list.End())
    {
        temp.InsertFirst(*it);
        it = list.Remove(it);
    }

    auto it2 = temp.Begin();

    while (it2 != temp.End())
        list.InsertLast(*it2++);    
}