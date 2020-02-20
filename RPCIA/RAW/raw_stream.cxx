#include "raw_stream.h"

#include <iostream>

RawStream::RawStream() : trigger_(true), current_word_index_(0) {
    RawDecoder::set_trigger_mode(trigger_);
    RawDecoder::set_pair_mode(true);
}

RawStream::~RawStream() {
}

void RawStream::set_reader(RawReader *raw_reader) {
    raw_reader_ = raw_reader;
}

bool RawStream::is_done() const {
    return false; // debug
}

bool RawStream::next(int &tdc, 
                     int &channel,
                     int &width,
                     int &bcid_tdc,
                     int &fine_time,
                     int &trigger_id,
                     int &bcid_fpga,
                     int &felix_counter) {
    if (is_done()) return false;

    Packet packet = raw_reader_->get_data();

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
    
    if (++current_word_index_ < packet.num_datawords()) {
        return true;
    } else {
        current_word_index_ = 0;
        return raw_reader_->next();
    }

    // compile_next_digits(); // Fill the buffer with stuff
    
    // RawDecoder::set_data(pb_)
    // tdc = RawDecoder::get_tdc(pb_.top(), current_word_index_);
    // channel = RawDecoder::get_channel(pb_.top(), current_word_index_);
    // width = RawDecoder::get_channel(pb_.top(), current_word_index_);
    // bcid_tdc = RawDecoder::get_bcid_tdc(pb_.top(), current_word_index_);
    // fine_time = RawDecoder::get_fine_time(pb_.top(), current_word_index_);
    // trigger_id = RawDecoder::get_trigger_id(pb_.top());
    // bcid_fpga = RawDecoder::get_bcid_fpga(pb_.top());
    // felix_counter = RawDecoder::get_felix_counter(pb_.top());

    // if (++current_word_index_ < RawDecoder::get_num_hits(pb_.top())) {
    //     return true;
    // }
    // pb_.pop_top(); // Go to next packet
    // if (pb_.top_size() > 0) { // If there is a next packet
    //     current_word_index_ = 0;
    //     return true;
    // }
    // current_word_index_ = 0; // If there is NO next packet in this event
    // pb_.next();
    // return false;
}

void RawStream::compile_next_digits() {
    // std::cout << "test" << std::endl;
    // while (pb_.get_range() < 10 && !raw_reader_->is_done()) {
    //     char * packet = raw_reader_->get_data();
    //     pb_.add(packet);
    // }
}