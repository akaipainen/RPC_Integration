#include "analysis_iterator.h"

template <class T>
AnalysisIterator<T>::AnalysisIterator(parent it)
 : Iterator<T>(it)
{
}

template <class T>
AnalysisIterator::AnalysisIterator<T> AnalysisIterator<T>::operator++()
{
    return AnalysisIterator(Iterator<T>::operator++());
}