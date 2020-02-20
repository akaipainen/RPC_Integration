#if !defined(OBJECT_PAIR_H)
#define OBJECT_PAIR_H

#include "TObject.h"

class ObjectPair : public TObject {
public:
    ObjectPair();
    ObjectPair(TObject *first, 
               TObject *second,
               Bool_t isOwnerOfFirst=true,
               Bool_t isOwnerOfSecond=false);
    ObjectPair(const ObjectPair &other);
    ObjectPair & operator=(const ObjectPair &other);
    
    virtual ~ObjectPair();

    /// Return the first element of the pair
    TObject* First() const { return fFirst; }
    /// Return  the second element of the pair
    TObject* Second() const { return fSecond; }

    /// Return the first element of the pair 
    TObject* Key() const { return fFirst; }
    /// Return the second element of the pair 
    TObject* Value() const { return fSecond; }

    virtual void Copy(TObject& other) const;
        
    virtual void Clear(Option_t* opt="");
  
private:

  TObject* fFirst; ///< first element of the pair
  TObject* fSecond; ///< second element of the pair
  Bool_t fIsOwnerOfFirst; ///< whether we own the first element
  Bool_t fIsOwnerOfSecond; ///<whether we own the second element
  
  ClassDef(ObjectPair,1) // A pair of TObject*
};

#endif // OBJECT_PAIR_H
