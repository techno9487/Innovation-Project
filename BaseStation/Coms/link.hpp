#pragma once

#include <openssl/evp.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

class DeviceLink
{
private:
    char key_bytes[16];
    char iv_bytes[16];
    bool isAlive;
    bool encryptionActive;

    EVP_CIPHER_CTX ctx_encryption;
    EVP_CIPHER_CTX ctx_decryption;

    int socket;
    sockaddr_in connectionInfo;
public:
    DeviceLink(int socket,sockaddr_in* addr,socklen_t& size); 
    void run();   
    int handleMessage(char* data,int length);
    int initEncryption();
};