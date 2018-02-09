/**
 Purpose: Handles all code to do with packets within the network
 
 Author: Toby Dunn
 Project: BCU Innocation Fest 2018
 */

#include <stdio.h>
#include "packet.hpp"

/**
 Packet to be sent every min to prove that the device is still alive
 */
struct PacketHeartbeat: public Packet
{
    PacketType GetType()
    {
        return PACKET_HEARTBEAT;
    }
    
    void Transmit(PacketStream &stream)
    {
        
    }
    
    void Receive(PacketStream &stream)
    {
        
    }
};

int main()
{
    PacketHeartbeat heartbeat;
    printf("%d\n",heartbeat.GetType());
    return 0;
}

