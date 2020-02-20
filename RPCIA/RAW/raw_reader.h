#if !defined(RAW_READER_H)
#define RAW_READER_H

#include <fstream>

#include "packet.h"

class RawReader {
private:
    std::fstream file_stream_;
    Packet current_packet_;
    char * data_;
    
public:
    RawReader();
    ~RawReader();

    // Open a file to be read
    bool open(const char* file);

    // Read the next packet, return false on failure
    bool next();

    // Get the packet data
    Packet get_data() const;

    // Reach end of file (EOF)?
    bool is_done();
};

#endif // RAW_READER_H
