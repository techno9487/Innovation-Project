#include "link.hpp"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include "json.hh"
#include <cppconn/prepared_statement.h>
#include <openssl/bio.h>
#include "crypto.h"

using json = nlohmann::json;

DeviceLink::DeviceLink(int socket,sockaddr_in* addr,socklen_t& size,DeviceThread* thread)
{
    this->isAlive = true;
    this->socket = socket;
    memcpy(&this->connectionInfo,addr,size);

    this->m_descriptor = thread;
}

void DeviceLink::run()
{
    //setup database access
    this->m_driver = get_driver_instance();
    if(this->m_driver == nullptr)
    {
        std::cout << "Failed to create driver instance for thread" << std::endl;
        return;
    }

    //make connection
    this->m_connection = this->m_driver->connect("127.0.0.1","root","password");
    if(this->m_connection == nullptr)
    {
        std::cout << "Failed to connect to the the database" << std::endl;
        return;
    }

    this->m_connection->setSchema("project");

    //sets up timeout
    timeval timeout_value = {};
    timeout_value.tv_sec = 10;
    
    if(setsockopt(socket,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout_value,sizeof(timeout_value)) < 0)
    {
        std::cout << "Failed to setup timeout" << std::endl;
    }
   
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
            std::string str = buffer.str();
            if(!handleMessage((char*)str.c_str(),strlen(str.c_str())))
            {
                //TODO: handle message error
            }

            //clear the buffer for next use
            buffer.str(std::string());
        }else{
            buffer.write(&c,1);
        }
    }
    
}

int DeviceLink::handleMessage(char* data, int length)
{
    if(encryptionActive)
    {
       //TODO: decrypt message 
    }

    try{
        json j = json::parse(data);

        std::string type = j["type"].get<std::string>();

        //TODO: better packet handling
        if(type == "initial")
        {
            if(j["device_id"].is_null())
            {
                return 0;
            }

            int device_id = j["device_id"].get<int>();
            std::cout << "Device with ID: " <<device_id << std::endl;

            //now to pull data for this device
            sql::PreparedStatement *stmt = this->m_connection->prepareStatement("SELECT DeviceKey,DeviceIV FROM Device where ID=?");
            stmt->setInt(1,device_id);
            sql::ResultSet *result = stmt->executeQuery();

            if(result == nullptr)
            {
                std::cout << "Unable to retrive device with ID: " << device_id << std::endl;
                //TODO: send error message that device can't be found
                delete stmt;
                return 0;
            }

            this->m_descriptor->iDeviceID = device_id;

            if(!result->next())
            {
                std::cout << "Unexpected error" << std::endl;
                delete result;
                delete stmt;
                return 0;
            }

            std::string key = result->getString(1);
            std::string iv = result->getString(2);

            delete result;
            delete stmt;

            decode((char*)key.c_str(),(unsigned char*)this->key_bytes);
            decode((char*)iv.c_str(),(unsigned char*)this->iv_bytes);

            json ack;
            ack["type"] = "ack";
            ack["success"] = true;

            std::string resp = ack.dump();
            sendmessage((char*)resp.c_str());

            this->encryptionActive = true;
        }
    }catch(nlohmann::detail::parse_error e)
    {
        return 0;
    }
    return 1;
}

int DeviceLink::decode(char* str,unsigned char* dest)
{
    BIO* bio = BIO_new_mem_buf(str,-1);
    BIO *b64 = BIO_new(BIO_f_base64());
    bio = BIO_push(b64,bio);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);

    int length = BIO_read(bio,dest,strlen(str));
    //std::cout << length << std::endl;

    BIO_free_all(bio);
    return 0;
}

void DeviceLink::sendmessage(char* raw)
{
    if(this->encryptionActive)
    {
        char* buffer = encrypt(raw,this->key_bytes,this->iv_bytes);
        if(buffer == nullptr)
        {
            return;
        }

        write(this->socket,buffer,strlen(buffer));
        delete buffer;
    }else{
        write(this->socket,raw,strlen(raw));
    }

    write(this->socket,"\n",1);
}