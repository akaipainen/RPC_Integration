#include "raw_reader.h"

#include <iostream>

RawReader::RawReader() { }

RawReader::~RawReader() { }

bool RawReader::open(const char* file) {
    file_stream_.open(file);
    return file_stream_.is_open();
}

bool RawReader::next() {
    char c;
    // Find the start of the next packet
    while (file_stream_.get(c)) {
        if (c == '=') break;
    }
    // If failed to find start of new packet, quit
    if (c != '=') {
        return false;
    }
    
    int num_bytes;
    file_stream_ >> num_bytes;
    // std::cout << num_bytes << std::endl;
    data_ = new char[num_bytes*2 + 1];
    data_[num_bytes*2] = '\0';

    // file_stream_.ignore(40); // ignore the header
    for (int i = 0; i < num_bytes; i++) {
        file_stream_.ignore(1);
        file_stream_.read(data_+i*2, 2);
    }

    bool ok = true;
    // std::cout << data_[2*num_bytes-2] << " " << data_[2*num_bytes-1] << std::endl;
    // If packet does not end in 'aa', this is an error. Log it.
    if (data_[2*num_bytes - 2] != 'a' || data_[2*num_bytes - 1] != 'a')
    {
        fprintf(stderr, "Error reading packet: %1.10s...\n", data_);
        ok = false;
    }
    // If number of bytes is < 24, this is an error. Log it
    if (num_bytes < 24) {
        fprintf(stderr, "Error reading packet: %1.10s...\n", data_);
        ok = false;
    }
    // If number of bytes is > 10000, this is an error. Log it
    if (num_bytes > 1000) {
        fprintf(stderr, "Error reading packet: %1.10s...\n", data_);
        ok = false;
    }
    
    // If ok, set current_packet_
    // Else, go to next packet
    if (ok) current_packet_ = Packet(data_);
    else return next();

    delete[] data_;
    data_ = nullptr;

    return true;
}

Packet RawReader::get_data() const { return current_packet_; }

bool RawReader::is_done() {
    return file_stream_.peek() == EOF;
}