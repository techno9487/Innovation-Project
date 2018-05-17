#include "link.hpp"
#include <iostream>
#include <sstream>
#include <unistd.h>

DeviceLink::DeviceLink(int socket,sockaddr* addr)
{
    this->isAlive = true;
    while(this->isAlive)
    {
        std::stringstream buffer;
        char c;
        while(true)
        {
            read(socket,&c,1);
            if(c == '\n')
            {
                break;
            }

            //TODO: handle the first non encrypted transfer that shares the device id, then make sure both ends are encrypting

            buffer.write(&c,1);
        }
    }
}

int DeviceLink::initEncryption()
{
    EVP_CIPHER_CTX_init(&this->ctx_encryption);
    EVP_EncryptInit_ex(&this->ctx_encryption,EVP_aes_128_cbc(),NULL,this->key_bytes,this->iv_bytes);
}