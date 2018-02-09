/**
 Purpose: Allow development/testing of arduino network code
 whilst using a desktop
 
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
        //TODO: add code to transmit heartbeat message
    }
    
    void Receive(PacketStream &stream)
    {
        //TODO: add code to decode packet
    }
};

int main()
{
    PacketHeartbeat heartbeat;
    printf("%d\n",heartbeat.GetType());
    return 0;
}

