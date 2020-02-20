#include "store.h"

#include "tree_manager.h"

ClassImp(Store);

Store::Store() { }

Store::~Store() { }

Store * Store::create(TTree &tree, const char *what) {
    TreeManager tree_man;

    TObject *obj = tman.create_object(tree, what);
    if (obj) {
        Store *c = dynamic_cast<Store *>(obj);
        return c;
    }
    return 0x0;
}

// TObject * Store::find_object(const char *name) const {
//   TIterator next(create_iterator());
//   TObject *obj;
  
//   while ((obj = next()))
//     if (!strcmp(name, obj->GetName())) return obj;
//   return 0;
// }

// TObject * Store::find_object(const TObject *object) const {
//     TIterator next(create_iterator());
//     TObject *obj;

//     while ((obj = next())) {
//         if (obj->IsEqual(object)) return obj;
//     }
//     return 0;
// }

// TObject * Store::find_object(int unique_id) const {
//   TIterator next(create_iterator());
//   TObject* obj;
//   while ( ( obj = next() ) ) 
//   {
//     if (obj->GetUniqueID() == uniqueID) return obj;
//   }
//   return 0x0;
// }