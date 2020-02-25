#if !defined(DIGIT_MAKER_H)
#define DIGIT_MAKER_H

#include "digit.h"
#include "raw_reader.h"
#include "raw_stream.h"
#include "store.h"
// class TriggerStore;

class DigitMaker {
private:
    RawStream *raw_stream_;
    Store<Digit> *digit_store_;

public:
    DigitMaker();
    ~DigitMaker();

    // Set the digit store to member variable
    // Fill the digit store with digits from raw_reader
    // Return the number of digits added
    int raw_to_digits(RawReader *raw_reader, Store<Digit> *digit_store);
};

#endif // DIGIT_MAKER_H
