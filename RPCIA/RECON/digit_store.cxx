#include "digit_store.h"

#include <TTree.h>

#include <iostream>
#include <vector>

#include "digit.h"
#include "tree_manager.h"

ClassImp(DigitStore);

DigitStore::DigitStore() : num_digits_(new Int_t(0)), trigger_id_(new Int_t(-1)) {
    digits_ = new std::vector<Digit>();
}

DigitStore::~DigitStore() {
    clear();
    delete trigger_id_;
    delete num_digits_;
}

Bool_t DigitStore::connect(TTree& tree) const {
    Bool_t ok = kTRUE;

    TreeManager tman;

    TBranch * branch = tree.GetBranch("digits");
    Bool_t is_making = (branch == 0);
    if (is_making) 
    {
        ok = ok && tman.make_branch(tree, "digits", ClassName(), digits_);
    } 
    else 
    {
        ok = ok && tman.set_address(tree, "digits", digits_);
    }

    branch = tree.GetBranch("trigger_id");
    is_making = (branch == 0);
    if (is_making)
    {
        tree.Branch("trigger_id", trigger_id_);
        // ok = ok && tman.make_branch(tree, "trigger_id", "Int_t", trigger_id_);
    }
    else
    {
        ok = ok && tman.set_address(tree, "trigger_id", trigger_id_);
    }

    branch = tree.GetBranch("num_digits");
    is_making = (branch == 0);
    if (is_making)
    {
        tree.Branch("num_digits", num_digits_);
        // ok = ok && tman.make_branch(tree, "trigger_id", "Int_t", trigger_id_);
    }
    else
    {
        ok = ok && tman.set_address(tree, "num_digits", num_digits_);
    }

    return ok;
}

Bool_t DigitStore::add(const Digit &digit) {
    digits_->push_back(Digit(digit));
    *trigger_id_ = digit.get_trigger_id();
    (*num_digits_)++;
    return true;
}

void DigitStore::clear() {
    digits_->clear();
    *num_digits_ = 0;
    // *trigger_id_ = -1;
}

