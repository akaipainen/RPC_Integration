#if !defined(RAW_STREAM_H)
#define RAW_STREAM_H

// #include "packet_buffer.h"


#include "packet.h"
#include "raw_reader.h"
#include "raw_decoder.h"

// Handles the raw reading of data from .dat files
class RawStream {
private:
    // PacketBuffer pb_;

    RawReader *raw_reader_;
    // RawDecoder *raw_decoder_;

    int current_word_index_;

    bool trigger_;

public:
    RawStream();
    ~RawStream();

    // Set the raw reader to read from
    void set_reader(RawReader *raw_reader);

    // Is the stream completely read?
    bool is_done() const;

    // Get the next stream of full packet information 
    // Return single hits until no more in this packet
    bool next(int &tdc, 
              int &channel,
              int &width,
              int &bcid_tdc,
              int &fine_time,
              int &trigger_id,
              int &bcid_fpga,
              int &felix_counter);

private:
    void compile_next_digits();
};

#endif // RAW_STREAM_H
