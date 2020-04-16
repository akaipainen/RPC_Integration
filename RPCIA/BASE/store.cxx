#include "store.h"

#include "digit.h"
#include "cluster.h"
#include "track.h"

template <class T>
Store<T>::Store(Bool_t master)
 : objects_(new std::vector<T *>())
 , trigger_id_(0)
 , master_(master)
{
}

template <class T>
Store<T>::~Store()
{
    clear();
    delete objects_;
}

template <class T>
Store<T>::Store(Store &other)
 : objects_(new std::vector<T *>())
 , trigger_id_(other.trigger_id_)
 , master_(false)
{
    if (master_)
    {
        std::copy(other.objects_->begin(), other.objects_->end(), objects_->begin());
    }
    else
    {
        *objects_ = *other.objects_;
    }
}

template <class T>
Int_t Store<T>::trigger_id() const
{
    return trigger_id_;
}

template <class T>
Bool_t Store<T>::connect(TTree &tree)
{
    TString name = get_branch_name();

    TBranch *branch = tree.GetBranch(name);
    Bool_t is_making = (branch == 0);
    if (is_making)
    {
        tree.Branch(name, objects_);
        return kTRUE;
    }
    else
    {
        tree.SetBranchAddress(name, &objects_);
        // branch->SetAddress(objects_);
        return kFALSE;
    }
}

template <class T>
Bool_t Store<T>::add(T &object)
{
    if (master_) // master
    {
        T * temp = new T(object);
        objects_->push_back(temp);
    }
    else // slave
    {
        objects_->push_back(&object);
    }
    trigger_id_ = object.trigger_id();
    return kTRUE;
}

template <class T>
void Store<T>::clear()
{
    // Only delete objects if this is the master
    if (master_)
    {
        for (auto &obj : *objects_)
        {
            delete obj;
        }   
        objects_->clear();
    }
}

// template <class T>
// Iterator<T> Store<T>::begin() const
// {
//     return Iterator<T>(objects_->cbegin());
// }

// template <class T>
// Iterator<T> Store<T>::end() const
// {
//     return Iterator<T>(objects_->cend());
// }

template <class T>
Iterator<T> Store<T>::begin() const
{
    return Iterator<T>(objects_->begin());
}

template <class T>
Iterator<T> Store<T>::end() const
{
    return Iterator<T>(objects_->end());
}

template <class T>
void Store<T>::remove(Iterator<T> it)
{
    typename std::vector<T *>::iterator pit = it;
    delete *pit;
    objects_->erase(pit);
}

template <class T>
Int_t Store<T>::size() const
{
    return objects_->size();
}

template <class T>
Bool_t Store<T>::empty() const
{
    return size() == 0;
}

template <class T>
TString Store<T>::get_branch_name() const
{
    TString name(T().ClassName());
    name = name.Append("s");
    name.ToLower();
    return name;
}

template class Store<Digit>;
template class Store<Cluster>;
template class Store<Track>;