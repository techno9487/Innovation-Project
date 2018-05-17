#include "link.hpp"
#include <iostream>
#include <sstream>
#include <unistd.h>

DeviceLink::DeviceLink(int socket,sockaddr_in* addr,socklen_t& size)
{
    this->isAlive = true;
    this->socket = socket;
    memcpy(&this->connectionInfo,addr,size);
}

void DeviceLink::run()
{
    //sets up timeout
    /*
    timeval timeout_value = {};
    timeout_value.tv_sec = 10;
    
    if(setsockopt(socket,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout_value,sizeof(timeout_value)) < 0)
    {
        std::cout << "Failed to setup timeout" << std::endl;
    }
    */

   
    std::stringstream buffer;
    char c;
    while(this->isAlive)
    {

        int count = read(this->socket,&c,1);

        //kill if other side closed connection
        if(count == 0)
        {
            std::cout << "Other side closed" << std::endl;
            break;
        }

        //see if there has been an error
        if(count == -1)
        {
            //kill the entire thing
            std::cout << "Killing thread; connection error." << std::endl;
            break;
        }
        

        //see if its ended
        if(c == '\n')
        {
            //TODO: handle the "message" being over then clear the buffer and start again
            std::cout << buffer.str() << std::endl;
            buffer.str(std::string());
        }else{
            buffer.write(&c,1);
        }
    }
    
}

int DeviceLink::initEncryption()
{
    //TODO: handle encrption
    //EVP_CIPHER_CTX_init(&this->ctx_encryption);
    //EVP_EncryptInit_ex(&this->ctx_encryption,EVP_aes_128_cbc(),NULL,this->key_bytes,this->iv_bytes);

    return 1;
}