/**
 Purpose: Allow development/testing of arduino network code
 whilst using a desktop
 
 Author: Toby Dunn
 Project: BCU Innocation Fest 2018
 */
#include "packet.hpp"

/**
 List of packet ID's
 */
enum PacketType
{
    PACKET_HEARTBEAT = 1,
    PACKET_SYNC,
};

/*
 Class to handle all incoming and outgoing comunication of packets
 */
struct PacketStream
{
    /**
     * ptr to the buffer for this stream object
     */
    char* buffer;
    /**
     * the maximum amount of data that the buffer can hold
     */
    size_t buffer_size;

    /**
     * current offset into the buffer that data will be read/written to/from
     */ 
    unsigned short index = 0;

    /**
     * Creates a new instance of a packet stream
     * 
     * @param buffer [in] pointer to the buffer for the stream to use
     * @param size [in] max size of the buffer to use 
     * */
    PacketStream(char* buffer,size_t size)
    {
        this->buffer = buffer;
        this->buffer_size = size;
        this->Reset();
    }

    /**
     * Resets the buffer ready to read/write a new packet
     */
    void Reset()
    {
        memset(buffer,0,buffer_size);
    }

    /**
     * Writes an integer into the stream
     * @param [in] value, the value to write into the stream
     */
    void WriteInt(int value)
    {
        //make sure theres room left, avoids a buffer overflow exploit
        if(index+sizeof(int) > buffer_size-1)
        {
            return;
        }

        memcpy(buffer+index,&value,sizeof(int));
        index += sizeof(int);
    }

    /**
     * Reads an int from the stream
     * @param [in] value, a pointer to an int to assign this value too
     */ 
    void ReadInt(int* value)
    {
        if(index+sizeof(int) > buffer_size-1)
        {
            *value = 0;
            return;
        }

        memcpy(value,buffer+index,sizeof(int));
    }
    
    /**
     * Writes raw data to the output stream
     * @param value [in], the value
     */
    void WriteData (char* value,size_t length)
    {
        if(index+length > buffer_size)
        {
            return;
        }
        
        memcpy(buffer+index,value,length);
        index += length;
    }
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


