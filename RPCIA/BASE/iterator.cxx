#include "iterator.h"

#include "digit.h"
#include "cluster.h"
#include "track.h"

template <class T>
Iterator<T>::Iterator(parent it)
 : parent(std::move(it))
{
}

// template <class T>
// Iterator<T>::Iterator(parent_const it)
//  : parent_const(std::move(it))
// {
// }

// template <class T>
// T & Iterator<T>::operator*()
// {
//     return *(parent::operator*());
// }

template <class T>
T & Iterator<T>::operator*()
{
    return *(parent::operator*());
}

// template <class T>
// T * Iterator<T>::operator->()
// {
//     return *(parent::operator->());
// }

template <class T>
T * Iterator<T>::operator->()
{
    return *(parent::operator->());
}

// template <class T>
// T * Iterator<T>::get()
// {
//     return parent::operator*();
// }

template <class T>
T * Iterator<T>::get()
{
    return parent::operator*();
}

template class Iterator<Digit>;
template class Iterator<Cluster>;
template class Iterator<Track>;