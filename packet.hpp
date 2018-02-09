/**
 Purpose: Handles all code to do with packets within the network
 
 Author: Toby Dunn
 Project: BCU Innocation Fest 2018
 */

/**
 List of packet ID's
 */
enum PacketType
{
    PACKET_HEARTBEAT = 1,
};

/*
 class to handle all incoming and outgoing comunication of packets
 */
struct PacketStream
{
    void WriteInt(int value);
    int ReadInt();
};

/*
 Base class for all packets sent within the smart home
 */
struct Packet
{
    /**
     needs to be implenebted to return the type of this packet
     
     @return the type of the packet
     */
    virtual PacketType GetType() = 0;
    
    /*
     Called whenever a packet needs to be serialized and sent to the network
     
     @param stream [in] the stream to send the packets data too
     */
    virtual void Transmit(PacketStream &stream) = 0;
    
    /**
     Called whenever a packet needs to be deserialized
     
     @param stream [in] the stream to read packet data from
     */
    virtual void Receive(PacketStream &stream) = 0;
};
