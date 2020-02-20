#if !defined(PACKET_H)
#define PACKET_H

#include <stdlib.h>

class Packet {
private:
    char * raw_data_;
    size_t size_;
    bool fragment_;

public:
    Packet(); // default ctor
    Packet(const char *packet_data);  // ctor
    Packet(const Packet &rhs); // copy ctor
    Packet(Packet &&rhs); // move ctor
    ~Packet(); // dtor
    Packet & operator=(const Packet & rhs); // copy assign
    Packet & operator=(Packet && rhs); // move assign

    // Get the size of the raw data packet
    size_t size() const;

    // Get the number of words in this packet
    size_t num_datawords() const;

    // Return the nth dataword
    unsigned long dataword(size_t n) const;

    // Return the fpga header
    unsigned long fpga_header() const;

    // Return `length` raw bits from `start` 
    // MAXIMUM LENGTH = 32 BITS
    // Little endian
    unsigned long raw_bits(size_t start, size_t length) const;

    // Return a const pointer to the `raw_data_` cstring
    const char * str() const;
};



#endif // PACKET_H
