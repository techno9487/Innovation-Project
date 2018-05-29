#pragma once

#include <openssl/evp.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <thread>

#include <cppconn/driver.h>
#include <cppconn/connection.h>

struct DeviceThread;

class DeviceLink
{
private:
    unsigned char key_bytes[16];
    unsigned char iv_bytes[16];
    bool isAlive;
    bool encryptionActive;

    int socket;
    sockaddr_in connectionInfo;

    sql::Driver *m_driver;
    sql::Connection *m_connection;

    DeviceThread* m_descriptor;

    int decode(char* str,unsigned char* dest);

    int handleMessage(char* data,int length);
    
public:
    DeviceLink(int socket,sockaddr_in* addr,socklen_t& size,DeviceThread* thread); 
    void run();   
    
    void sendmessage(char* raw);
};

struct DeviceThread
{
    DeviceLink* link;
    int iDeviceID;
    std::thread thread;
};