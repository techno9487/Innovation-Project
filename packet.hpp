/**
 Purpose: Handles all code to do with packets within the network
 
 Author: Toby Dunn
 Project: BCU Innocation Fest 2018
 */

/**
 * The max size of a packet can only be 1024 bytes, this is to make sure
 * that no dynamic memory has to be allocated.
 */
char packet_buffer[1024];

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
    public PacketStream(char* buffer,size_t size)
    {
        this->buffer = buffer;
        this->buffer_size = size;
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
