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

class DeviceLink
{
private:
    char key_bytes[16];
    char iv_bytes[16];
    bool isAlive;
    bool encryptionActive;

    int socket;
    sockaddr_in connectionInfo;

    sql::Driver *m_driver;
    sql::Connection *m_connection;

    int decode(char* str,unsigned char* dest);

    //TODO set up the encryption/decryption functions
public:
    DeviceLink(int socket,sockaddr_in* addr,socklen_t& size); 
    void run();   
    int handleMessage(char* data,int length);
};

struct DeviceThread
{
    DeviceLink* link;
    int iDeviceID;
    std::thread thread;
};