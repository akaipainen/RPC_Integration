#include "reconstructor.h"

#include "digit_maker.h"
#include "digit_store.h"
#include "raw_reader.h"

#include <TTree.h>

Reconstructor::Reconstructor() { }
Reconstructor::~Reconstructor() { }

void Reconstructor::convert_digits(RawReader *raw_reader, DigitStore *digit_store) const {
    create_digit_maker();
    digit_maker_->raw_to_digits(raw_reader, digit_store);
}

void Reconstructor::reconstruct(RawReader *raw_reader, TTree *digits_tree) const {
    if (!digits_tree) {
        return;
    }
    
    convert_digits(raw_reader, get_digit_store(), get_trigger_store());
    fill_tree(get_trigger_store(), *digits_tree);
}

void Reconstructor::fill_tree(DigitStore *digit_store, TTree &digits_tree) const {
    
}

void Reconstructor::create_digit_maker() {
    if (!digit_maker_) {
        digit_maker_ = new DigitMaker();
    }
}