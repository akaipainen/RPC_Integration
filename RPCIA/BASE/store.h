#if !defined(STORE_H)
#define STORE_H

#include <vector>

#include <TTree.h>
#include <TObject.h>
#include <TIterator.h>
#include <TString.h>

template <class T>
class Store 
{
private:
    std::vector<T *> *objects_;
    Int_t *trigger_id_;

public:
    Store();
    ~Store();

    // Connect this store to a TTree
    Bool_t connect(TTree &tree);

    // Add a TObject to the store, if it can be downcast
    Bool_t add(const TObject &object);

    // Add an Obj to the store
    Bool_t add(const T &obj);

    // Clear all objects from store
    void clear();

    // Begin iterator to loop over all objects
    typename std::vector<T *>::const_iterator cbegin() const;

    // End iterator to loop over all objects
    typename std::vector<T *>::const_iterator cend() const;

    // Begin non const iterator
    typename std::vector<T *>::iterator begin();

    // End non const iterator
    typename std::vector<T *>::iterator end();

    // Remove an object from the store
    void remove(typename std::vector<T *>::iterator it);

    // Get the number of objects stored
    Int_t size() const;

    // Whether the store is empty or not
    Bool_t empty() const;

private:
    // Get the branch name of the store's objects
    TString get_branch_name() const;
};

#endif // STORE_H