#if !defined(ITERATOR_H)
#define ITERATOR_H

#include <vector>

template <class T>
class Iterator : public std::vector<T *>::iterator
{
protected:
    using parent = typename std::vector<T *>::iterator;
    // using parent_const = typename std::vector<T *>::const_iterator;

public:
    Iterator(parent it);
    // Iterator(parent_const it);

    T & operator*();

    // T & operator*() const;

    T * operator->();

    // T * operator->() const;

    T * get();

    // T * get() const;
};


#endif // ITERATOR_H
