#include "digit_store.h"

#include <TTree.h>

#include <iostream>
#include <vector>

#include "digit.h"
#include "tree_manager.h"

ClassImp(DigitStore);

DigitStore::DigitStore() : num_digits_(0) {
    digits_ = new std::vector<Digit>();
}

DigitStore::~DigitStore() {
    clear();
}

Bool_t DigitStore::connect(TTree& tree) const {
    Bool_t ok = kTRUE;

    TreeManager tman;
    TBranch * branch = tree.GetBranch("digits");

    Bool_t is_making = (branch == 0);
    if (is_making) {
        ok = ok && tman.make_branch(tree, "digits", ClassName(), digits_);
    } else {
        ok = ok && tman.set_address(tree, "digits", digits_);
    }

    return ok;
}

Bool_t DigitStore::add(const Digit &digit) {
    digits_->push_back(Digit(digit));
    return true;
}

void DigitStore::clear() {
    digits_->clear();
}

