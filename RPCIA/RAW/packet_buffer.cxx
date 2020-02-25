#include "packet_buffer.h"

#include <iterator>
#include <iostream>

#include "raw_decoder.h"

// Function overload to make my life easier
const auto& id = RawDecoder::decode_trigger_id;

PacketBuffer::PacketBuffer()
{
}

PacketBuffer::~PacketBuffer()
{
}

void PacketBuffer::insert(const Packet &packet)
{
    auto it = buffer_.end();
    while (it != buffer_.begin()
           && id(packet) < id(prev(it)->front()))
    {
        std::advance(it, -1);
    }
    if (it != buffer_.begin() 
        && id(packet) == id(prev(it)->front()))
    {
        prev(it)->push_back(packet);
    }
    else
    {
        std::list<Packet> new_trigger;
        new_trigger.push_back(packet);
        buffer_.insert(it, new_trigger);
    }
}

const Packet& PacketBuffer::front() const 
{
    return buffer_.front().front();
}

bool PacketBuffer::pop()
{
    if (buffer_.front().size() == 1)
    {
        buffer_.pop_front();
        return false;
    }

    buffer_.front().pop_front();
    return true;
}

size_t PacketBuffer::size() const
{
    return buffer_.size();
}

const Packet& PacketBuffer::back() const
{
    return buffer_.back().back();
}