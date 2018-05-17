#pragma once

#include <openssl/evp.h>

class DeviceLink
{
private:
    char key_bytes[16];
    char iv_bytes[16];
    bool isAlive;
    bool encryptionActive;

    EVP_CIPHER_CTX ctx_encryption;
    EVP_CIPHER_CTX ctx_decryption;
public:
    DeviceLink(int socket,sockaddr* addr);    
    int initEncryption();
};