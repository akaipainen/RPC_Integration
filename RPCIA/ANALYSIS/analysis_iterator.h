#if !defined(ANALYSIS_ITERATOR_H)
#define ANALYSIS_ITERATOR_H

#include "TTree.h"

#include "iterator.h"

template <class T>
class AnalysisIterator : public Iterator<T>
{
public:
    AnalysisIterator(parent it);

    AnalysisIterator<T> operator++();
    
    AnalysisIterator<T> operator++(int);
    
    AnalysisIterator<T> operator--();
    
    AnalysisIterator<T> operator--(int);

};



#endif // ANALYSIS_ITERATOR_H
