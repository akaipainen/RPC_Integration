#include "raw_stream.h"

#include <iostream>

RawStream::RawStream() : trigger_(true), current_word_index_(0) {
    RawDecoder::set_trigger_mode(trigger_);
}

RawStream::~RawStream() {
}

void RawStream::set_reader(RawReader *raw_reader) {
    raw_reader_ = raw_reader;
}

bool RawStream::is_done() const {
    return pb_.size() == 0;
}

bool RawStream::next(int &tdc, 
                     int &channel,
                     int &width,
                     int &bcid_tdc,
                     int &fine_time,
                     int &trigger_id,
                     int &bcid_fpga,
                     int &felix_counter) {
    fill_buffer();

    if (is_done()) return false;

    Packet packet = pb_.front();

    RawDecoder::decode(packet,
                       current_word_index_,
                       tdc,
                       channel,
                       width,
                       bcid_tdc,
                       fine_time,
                       trigger_id,
                       bcid_fpga);

    // std::cout << current_word_index_ <<
    //       " " << packet.num_datawords() <<
    //       " " << packet.size() <<
    //       " " << tdc <<
    //       " " << channel <<
    //       " " << width <<
    //       " " << packet.dataword(current_word_index_) << '\n';
    
    if (++current_word_index_ < packet.num_datawords()) 
    {
        return true;
    }
    current_word_index_ = 0;
    return pb_.pop();
}

void RawStream::fill_buffer() {
    while (pb_.size() < 10 && !raw_reader_->is_done())
    {
        Packet packet = raw_reader_->get_data();
        pb_.insert(packet);
        raw_reader_->next();
    }
}