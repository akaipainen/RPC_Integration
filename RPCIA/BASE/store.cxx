#include "store.h"

#include "digit.h"
#include "cluster.h"
#include "track.h"

template <class T>
Store<T>::Store()
 : objects_(new std::vector<T *>())
 , trigger_id_(new Int_t(0))
{
}

template <class T>
Store<T>::~Store()
{
    clear();
    delete objects_;
    delete trigger_id_;
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
Bool_t Store<T>::add(const TObject &object)
{
    const T *obj = dynamic_cast<const T*>(&object);
    if (obj)
    {
        return add(*obj);
    }
    return kFALSE;
}

template <class T>
Bool_t Store<T>::add(const T &object)
{
    objects_->push_back(new T(object));
    // objects_->back()->AddDirectory(0);
    *trigger_id_ = object.trigger_id();
    return kTRUE;
}

template <class T>
void Store<T>::clear()
{
    for (auto &obj : *objects_)
    {
        delete obj;
    }
    objects_->clear();
}

template <class T>
typename std::vector<T *>::const_iterator Store<T>::cbegin() const
{
    return objects_->cbegin();
}

template <class T>
typename std::vector<T *>::const_iterator Store<T>::cend() const
{
    return objects_->cend();
}

template <class T>
typename std::vector<T *>::iterator Store<T>::begin()
{
    return objects_->begin();
}

template <class T>
typename std::vector<T *>::iterator Store<T>::end()
{
    return objects_->end();
}

template <class T>
void Store<T>::remove(typename std::vector<T *>::iterator it)
{
    delete *it;
    objects_->erase(it);
}

template <class T>
TString Store<T>::get_branch_name() const
{
    TString name(T().ClassName());
    name = name.Append("s");
    name.ToLower();
    return name;
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

template class Store<Digit>;
template class Store<Cluster>;
template class Store<Track>;