#if !defined(RAW_DECODER_H)
#define RAW_DECODER_H

#include "noisy_rollover_counter.h"
#include "packet.h"

class RawDecoder {
private:
    static RawDecoder instance_;

    bool pair_mode_;
    bool trigger_;

    NoisyRolloverCounter id_counter_;

public:
    // Delete copy and assignment operators
    RawDecoder(RawDecoder const& copy) = delete;
    RawDecoder& operator=(RawDecoder const& copy) = delete;

    // Get the singleton instance of RawDecoder
    static RawDecoder& get();

    // Set the decoder to pair mode
    static void set_pair_mode(bool value=true);

    // Set the decoder to trigger mode
    static void set_trigger_mode(bool value=true);

    // Completely decode a packet into reference variables
    static void decode(const Packet &packet, 
                       const size_t word,
                       int &tdc, 
                       int &channel,
                       int &width,
                       int &bcid_tdc,
                       int &fine_time,
                       int &trigger_id,
                       int &bcid_fpga);

    // Decode the tdc
    static unsigned long decode_tdc(const Packet &packet, size_t word);

    // Decode the channel number
    static unsigned long decode_channel(const Packet &packet, size_t word);
    
    // Decode the width (if pair mode, otherwise returns max value)
    static unsigned long decode_width(const Packet &packet, size_t word);
    
    // Decode the bcid from tdc
    static unsigned long decode_bcid_tdc(const Packet &packet, size_t word);
    
    // Decode the fine time
    static unsigned long decode_fine_time(const Packet &packet, size_t word);

    // Decode the number of hits
    static unsigned long decode_num_hits(const Packet &packet);

    // Decode the trigger id
    static unsigned long decode_trigger_id(const Packet &packet);

    // Decode the bcid from fpga
    static unsigned long decode_bcid_fpga(const Packet &packet);

private:
    RawDecoder() : id_counter_(256) {}

    void set_pair_mode_imp(bool value);
    void set_trigger_mode_imp(bool value);

    unsigned long decode_tdc_imp(const Packet &packet, size_t word);
    unsigned long decode_channel_imp(const Packet &packet, size_t word);
    unsigned long decode_width_imp(const Packet &packet, size_t word);
    unsigned long decode_bcid_tdc_imp(const Packet &packet, size_t word);
    unsigned long decode_fine_time_imp(const Packet &packet, size_t word);
    unsigned long decode_num_hits_imp(const Packet &packet);
    unsigned long decode_trigger_id_imp(const Packet &packet);
    unsigned long decode_bcid_fpga_imp(const Packet &packet);

    // Helper function to slice bits from datawords
    unsigned long slice_bits(unsigned long bits, size_t start, size_t length);
};

#endif // RAW_DECODER_H
