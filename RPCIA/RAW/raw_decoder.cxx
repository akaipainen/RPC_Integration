#include "raw_decoder.h"

// ############ Singleton functions ###########

RawDecoder RawDecoder::instance_;

RawDecoder& RawDecoder::get() {
    return instance_;
}

// ############# Static functions ###########

// -------------- Settings ------------------

void RawDecoder::set_pair_mode(bool value) {
    get().set_pair_mode_imp(value);
}

void RawDecoder::set_trigger_mode(bool value) {
    get().set_trigger_mode_imp(value);
}

// ------------ Decode functions -------------

void RawDecoder::decode(const Packet &packet, 
                        const size_t word,
                        int &tdc, 
                        int &channel,
                        int &width,
                        int &bcid_tdc,
                        int &fine_time,
                        int &trigger_id,
                        int &bcid_fpga)
{
    tdc = decode_tdc(packet, word);
    channel = decode_channel(packet, word);
    width = decode_width(packet, word);
    bcid_tdc = decode_bcid_tdc(packet, word);
    fine_time = decode_fine_time(packet, word);
    trigger_id = decode_trigger_id(packet);
    bcid_fpga = decode_bcid_fpga(packet);
}

unsigned long RawDecoder::decode_tdc(const Packet &packet, size_t word) {
    return get().decode_tdc_imp(packet, word);
}

unsigned long RawDecoder::decode_channel(const Packet &packet, size_t word) {
    return get().decode_channel_imp(packet, word);
}

unsigned long RawDecoder::decode_width(const Packet &packet, size_t word) {
    return get().decode_width_imp(packet, word);
}

unsigned long RawDecoder::decode_bcid_tdc(const Packet &packet, size_t word) {
    return get().decode_bcid_tdc_imp(packet, word);
}

unsigned long RawDecoder::decode_fine_time(const Packet &packet, size_t word) {
    return get().decode_fine_time_imp(packet, word);
}

unsigned long RawDecoder::decode_num_hits(const Packet &packet) {
    return get().decode_num_hits_imp(packet);
}

unsigned long RawDecoder::decode_trigger_id(const Packet &packet) {
    return get().decode_trigger_id_imp(packet);
}

unsigned long RawDecoder::decode_bcid_fpga(const Packet &packet) {
    return get().decode_bcid_fpga_imp(packet);
}

// ########## Implementations ###########

// ------------- Settings --------------

void RawDecoder::set_pair_mode_imp(bool value) {
    pair_mode_ = value;
}

void RawDecoder::set_trigger_mode_imp(bool value) {
    trigger_ = value;
}

// -------------- Decode ----------------

unsigned long RawDecoder::decode_tdc_imp(const Packet &packet, size_t word) {
    return slice_bits(packet.dataword(word), 24, 4);
}

unsigned long RawDecoder::decode_channel_imp(const Packet &packet, size_t word) {
    return slice_bits(packet.dataword(word), 19, 5);
}

unsigned long RawDecoder::decode_width_imp(const Packet &packet, size_t word) {
    if (pair_mode_) return slice_bits(packet.dataword(word), 12, 7);
    return -1;
}

unsigned long RawDecoder::decode_bcid_tdc_imp(const Packet &packet, size_t word) {
    if (pair_mode_) return slice_bits(packet.dataword(word), 7, 5);
    return slice_bits(packet.dataword(word), 7, 12);
}

unsigned long RawDecoder::decode_fine_time_imp(const Packet &packet, size_t word) {
    return slice_bits(packet.dataword(word), 0, 7);
}

unsigned long RawDecoder::decode_num_hits_imp(const Packet &packet) {
    return slice_bits(packet.fpga_header(), 24, 8);
}

unsigned long RawDecoder::decode_trigger_id_imp(const Packet &packet) {
    return id_counter_.get(slice_bits(packet.fpga_header(), 16, 8));
}

unsigned long RawDecoder::decode_bcid_fpga_imp(const Packet &packet)
{
    return slice_bits(packet.fpga_header(), 0, 16);
}

// ############# Helpers ##################

unsigned long RawDecoder::slice_bits(unsigned long bits, size_t shift, size_t length)
{
    return bits >> shift & (1 << length) - 1;
}