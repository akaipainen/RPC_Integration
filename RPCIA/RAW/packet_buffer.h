#if !defined(PACKET_BUFFER_H)
#define PACKET_BUFFER_H

#include <list>

#include "packet.h"

class PacketBuffer
{
private:
    std::list<std::list<Packet>> buffer_;
    
public:
    PacketBuffer();
    ~PacketBuffer();

    // Insert a packet into the buffer
    void insert(const Packet &packet);

    // Get the front element of buffer
    const Packet& front() const;

    // Pop off the top element of the buffer
    // Return false if this was the last packet with this id
    bool pop();

    // Get the size of the buffer
    size_t size() const;

private:
    // Get the back element of the buffer
    const Packet& back() const;
};

#endif // PACKET_BUFFER_H
