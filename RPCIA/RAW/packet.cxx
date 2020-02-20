#include "packet.h"

#include <algorithm>
#include <cstring>
#include <iostream>

Packet::Packet()
 : raw_data_(new char[1]), size_(1)
{
}

Packet::Packet(const char *packet_data)
 : size_(strlen(packet_data))
{
    // std::cout << "Packet size: " << size_ << std::endl;
    raw_data_ = new char[size_ + 1];
    strcpy(raw_data_, packet_data);
    raw_data_[size_] = '\0';
    // std::cout << raw_data_ << std::endl;
}

Packet::Packet(const Packet &rhs)
 : size_(rhs.size_)
{
    raw_data_ = new char[size_ + 1];
    strcpy(raw_data_, rhs.raw_data_);
}

Packet::Packet(Packet &&rhs)
 : raw_data_(nullptr), size_(0)
{
    size_ = rhs.size_;
    raw_data_ = rhs.raw_data_;

    rhs.raw_data_ = nullptr;
    rhs.size_ = 0;
}

Packet::~Packet()
{
    delete[] raw_data_;
}

Packet & Packet::operator=(const Packet & rhs)
{
    if (this != &rhs)
    {
        Packet temp(rhs);
        std::swap(raw_data_, temp.raw_data_);
        std::swap(size_, temp.size_);
    }
    return *this;
}

Packet & Packet::operator=(Packet &&rhs)
{
    if (this != &rhs)
    {
        delete[] raw_data_;
        raw_data_ = rhs.raw_data_;
        size_ = rhs.size_;

        rhs.raw_data_ = nullptr;
        rhs.size_ = 0;
    }
    return *this;
}

// In hex digits
size_t Packet::size() const 
{
    return size_;
}

size_t Packet::num_datawords() const
{
    return (size()/2 - 20) / 4;
}

unsigned long Packet::dataword(size_t n) const
{
    const char * start_ptr = raw_data_ + 26 + n * 8;
    char temp[9];
    strncpy(temp, start_ptr, 8);
    temp[8] = '\0';
    // printf("%s = %lu\n", temp, strtoul(temp, nullptr, 16));
    return strtoul(temp, nullptr, 16);
}

unsigned long Packet::fpga_header() const
{
    char * start_ptr = raw_data_ + 26 + num_datawords() * 8; // 8 hex bytes = 32 bits
    char temp[9]; // 8 + 1 for null term char
    strncpy(temp, start_ptr, 8);
    temp[8] = '\0';
    // printf("%s = %lu\n", temp, strtoul(temp, nullptr, 16));
    return strtoul(temp, nullptr, 16);
}

unsigned long Packet::raw_bits(size_t start, size_t length) const
{
    if (start + length > size())
    {
        return -1;
    }
    
    char * start_ptr = raw_data_ + start;
    char temp[length + 1];
    strncpy(temp, start_ptr, length);
    temp[length] = '\0';
    // printf("%s = %lu\n", temp, strtoul(temp, nullptr, 16));
    return strtoul(temp, nullptr, 16);
}

const char * Packet::str() const {
    return raw_data_;
}