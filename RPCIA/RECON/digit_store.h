#if !defined(DIGIT_STORE_H)
#define DIGIT_STORE_H

// #include "store.h"

#include <vector>

#include <TObject.h>
#include <TTree.h>
#include <TIterator.h>
#include <TClonesArray.h>

#include "digit.h"

class DigitStore : public TObject {
private:
    std::vector<Digit> *digits_;
    Int_t num_digits_;
    
public:
    DigitStore();
    ~DigitStore();

    // Connect this DigitStore to the 'digits' branch of the tree
    // If it does not exist, create it
    Bool_t connect(TTree &tree) const;

    // Add a digit
    Bool_t add(const Digit &digit);

    // Clear all digits
    void clear();

    // Create an iterator to loop over all digits
    TIterator * create_iterator();

    // Create an iterator to loop over all digits of a given event
    TIterator * create_iterator(Int_t event_id);

    // Get the number of digits stored
    Int_t size();

    // Get the number of digits stored in given event id
    Int_t size(Int_t event);

private:
    ClassDef(DigitStore, 1);
};

#endif // DIGIT_STORE_H
