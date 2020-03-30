#if !defined(DIGIT_MAKER_H)
#define DIGIT_MAKER_H

#include <vector>

#include "digit.h"
#include "raw_reader.h"
#include "raw_stream.h"
#include "store.h"
// class TriggerStore;

class DigitMaker {
private:
    RawStream *raw_stream_;
    Store<Digit> *digit_store_;

    std::vector<int> *noisy_strips_;

public:
    DigitMaker();
    ~DigitMaker();

    // Set noisy strips
    void set_noisy_strips(std::vector<int> *noisy_strips);

    // Set the digit store to member variable
    // Fill the digit store with digits from raw_reader
    // Return the number of digits added
    int raw_to_digits(RawReader *raw_reader, Store<Digit> *digit_store);

private:
    // Return if a strip is noisy
    bool noisy(const int tdc, const int strip);
};

#endif // DIGIT_MAKER_H
