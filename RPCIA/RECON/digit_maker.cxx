#include "digit_maker.h"

#include "raw_reader.h"
#include "raw_stream.h"

DigitMaker::DigitMaker() {
    raw_stream_ = new RawStream();
    noisy_strips_ = nullptr;
}
DigitMaker::~DigitMaker() {
    delete raw_stream_;
}

void DigitMaker::set_noisy_strips(std::vector<int> *noisy_strips)
{
    noisy_strips_ = noisy_strips;
}

int DigitMaker::raw_to_digits(RawReader *raw_reader, Store<Digit> *digit_store)
{
    if (!digit_store) {
        return -1;
    }
    
    digit_store_ = digit_store;
    digit_store_->clear();

    raw_stream_->set_reader(raw_reader);

    Int_t tdc;
    Int_t channel;
    Int_t width;
    Int_t bcid_tdc;
    Int_t fine_time;

    Int_t trigger_id;
    Int_t bcid_fpga;
    Int_t felix_counter;

    int new_digit_count = 0;
    bool is_more = false;
    do 
    {
        is_more = raw_stream_->next(tdc, channel, width, bcid_tdc, 
                                    fine_time, trigger_id, bcid_fpga, 
                                    felix_counter);
        Digit digit(trigger_id, bcid_fpga, felix_counter, tdc, 
                    channel, width, bcid_tdc, fine_time);
        if (!noisy(digit.tdc(), digit.strip()))
        {
            digit_store_->add(digit);
            new_digit_count++;
        }
    } while (is_more);
    return new_digit_count;
}

bool DigitMaker::noisy(const int tdc, const int strip)
{
    if (!noisy_strips_)
    {
        return false;
    }
    return std::find(noisy_strips_->begin(), noisy_strips_->end(), tdc*32+strip) != noisy_strips_->end();
}