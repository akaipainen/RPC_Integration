#if !defined(STORE_H)
#define STORE_H

#include <vector>

#include <TTree.h>
#include <TObject.h>
#include <TIterator.h>
#include <TString.h>

#include "iterator.h"

template <class T>
class Store 
{
private:
    std::vector<T *> *objects_;
    Int_t trigger_id_;
    Bool_t master_;

public:
    Store(Bool_t master=false);
    ~Store();
    Store(Store &other);

    // Get the trigger id
    Int_t trigger_id() const;

    // Connect this store to a TTree
    Bool_t connect(TTree &tree);

    // Add an Obj to the store
    Bool_t add(T &obj);

    // Clear and delete all objects from store
    void clear();

    // Begin const iterator
    Iterator<T> begin() const;

    // End const iterator
    Iterator<T> end() const;

    // Return front object
    T* front() const
    {
        return objects_->front();
    }

    // Return back object
    T* back() const
    {
        return objects_->back();
    }

    // Remove an object from the store
    void remove(Iterator<T> it);

    // Get the number of objects stored
    Int_t size() const;

    // Whether the store is empty or not
    Bool_t empty() const;

private:
    // Get the branch name of the store's objects
    TString get_branch_name() const;
};

#endif // STORE_H