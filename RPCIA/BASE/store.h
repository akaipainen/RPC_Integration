#if !defined(STORE_H)
#define STORE_H

class TTree;
class TObject;
class TIterator;

class Store : public TObject {
public:
    Store();
    ~Store();

    // Add an item to the store
    virtual Bool_t add(TObject *object) = 0;

    // Clear contents
    virtual void clear() = 0;

    // Create an empty copy of this store
    virtual Store * create() = 0;

    // Create a store from a TTree
    // static Store * create(TTree& tree, const char *what);

    // Return an iterator to loop over the whole store
    virtual TIterator * create_iterator() const = 0;
    
    // Whether the connect(TTree&) method is implemented
    // virtual bool can_connect() const = 0;
    
    // Connect this store to a TTree
    virtual Bool_t connect(TTree& tree, Bool_t alone=true) const;

    // Find an object by name
    // virtual TObject * find_object(const char *name) const;
    
    // Find an object
    // virtual TObject * find_object(const TObject* object) const;

    // Find an object using a single id
    // virtual TObject * find_object(int unique_id) const;
        
    // The number of objects stored
    virtual Int_t size() const = 0;

    // Whether the store is empty or not
    virtual Bool_t empty() const { return size() == 0; }

private:
    ClassDef(Store, 1);
};

#endif // STORE_H
