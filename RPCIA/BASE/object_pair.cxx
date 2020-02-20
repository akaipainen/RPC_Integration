#include "object_pair.h"

ClassImp(ObjectPair);

ObjectPair::ObjectPair() 
: TObject(), fFirst(0x0), fSecond(0x0), fIsOwnerOfFirst(kTRUE), fIsOwnerOfSecond(kTRUE) { }

ObjectPair::ObjectPair(TObject* first, TObject* second,
                       Bool_t isOwnerOfFirst, Bool_t isOwnerOfSecond)
: TObject(), fFirst(first), fSecond(second), fIsOwnerOfFirst(isOwnerOfFirst), fIsOwnerOfSecond(isOwnerOfSecond) { }

ObjectPair::ObjectPair(const ObjectPair& other)
: TObject(other), fFirst(0x0), fSecond(0x0), fIsOwnerOfFirst(kTRUE), fIsOwnerOfSecond(kTRUE) {
    other.Copy(*this);
}

ObjectPair& ObjectPair::operator=(const ObjectPair& other) {
    if ( this != &other)  {
        other.Copy(*this);
    }
    return *this;
}

ObjectPair::~ObjectPair() {
    if ( fIsOwnerOfFirst ) delete fFirst;
    if ( fIsOwnerOfSecond ) delete fSecond;
}

void ObjectPair::Clear(Option_t*) {
    if ( fIsOwnerOfFirst ) delete fFirst;
    if ( fIsOwnerOfSecond ) delete fSecond;
    fFirst = 0x0;
    fSecond = 0x0;
}

void ObjectPair::Copy(TObject& other) const {
    TObject::Copy(other);
    ObjectPair &pair = (ObjectPair&)(other);
    pair.fIsOwnerOfFirst = fIsOwnerOfFirst;
    pair.fIsOwnerOfSecond = fIsOwnerOfSecond;
    if ( fIsOwnerOfFirst ) {
        pair.fFirst = fFirst->Clone();
    }
    else {
        pair.fFirst = fFirst;
    }
    if ( fIsOwnerOfSecond ) {
        pair.fSecond = fSecond->Clone();
    }
    else {
        pair.fSecond = fSecond;
    }
}