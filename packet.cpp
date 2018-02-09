/**
 Purpose: Allow development/testing of arduino network code
 whilst using a desktop
 
 Author: Toby Dunn
 Project: BCU Innocation Fest 2018
 */

#include <stdio.h>
#include "packet.hpp"

/**
 * The max size of a packet can only be 1024 bytes, this is to make sure
 * that no dynamic memory has to be allocated.
 */
char packet_buffer[1024];

/**
 Packet to be sent every min to prove that the device is still alive
 */
struct PacketHeartbeat: public Packet
{
    /**
     * Token to be echoed back by the base station
     */
    int token = 255;

    PacketType GetType()
    {
        return PACKET_HEARTBEAT;
    }
    
    void Transmit(PacketStream &stream)
    {
        stream.WriteInt(token);
    }
    
    void Receive(PacketStream &stream)
    {
        stream.ReadInt(&token);
    }
};

int main()
{
    PacketStream stream(packet_buffer,1024);

    return 0;
}

