/*
    Author: Toby Dunn

    Client software for the innovation project
*/

#include <iostream>
#include "json.hh"
#include <sys/socket.h>
#include <unistd.h>
#include "config.h"
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <fstream>
#include "crypto.h"

using json = nlohmann::json;

//global program config file
json config;

void sendmessage(int fd,char* raw);
void handleLocalMessage(std::string str,int fd);

struct ClientDevice
{
    unsigned char key[16];
    unsigned char iv[16];
    int device_id;
    bool encrypted = false;
    int fd;
};

void loadConfig()
{
    std::ifstream configFile("config.json",std::ios::in);
    if(!configFile.is_open())
    {
        return;
    }

    configFile >> config;
}

ClientDevice current_device;

//populates the current_device field from the values in the config file
bool load_info_from_config()
{
    if(config["device_id"].is_null())
    {
        std::cout << "Error: config doesn;t contain device ID!" << std::endl;
        return false;
    }

    if(config["key"].is_null())
    {
        std::cout << "Error: config doesn't contiain key" << std::endl;
        return false;
    }

    if(config["iv"].is_null())
    {
        std::cout << "Error: config doesn't contain IV" << std::endl;
        return false;
    }

    current_device.device_id = config["device_id"].get<int>();

    std::string key = config["key"].get<std::string>();
    decode_key((char*)key.c_str(),current_device.key);

    std::string iv = config["iv"].get<std::string>();
    decode_key((char*)iv.c_str(),current_device.iv);

    return true;
}

//main loop for the client's connection to the server
void client_loop();

int main()
{
    loadConfig();
    if(!load_info_from_config())
    {
        return 1;
    }else{
        std::cout << "Loaded config file" << std::endl;
    }

    int fd = socket(AF_INET,SOCK_STREAM,0);

    sockaddr_in address = {};
    address.sin_family = AF_INET;
    address.sin_port = htons(1025);
    address.sin_addr.s_addr = inet_addr(SERVER_IP);

    if(connect(fd,(sockaddr*)&address,sizeof(address)) != 0)
    {
        std::cout << "Failed to connect to server" << std::endl;
        return 1;
    }

    std::cout << "Connected!" << std::endl;
    current_device.fd = fd;
    
    //starts the client_loop
    client_loop();

    return 0;
}

void client_loop()
{
    json first;
    first["type"] = "initial";
    first["device_id"] = current_device.device_id;

    std::string raw = first.dump();
    sendmessage(current_device.fd,(char*)raw.c_str());

    std::stringstream buffer;
    char c;
    while(true)
    {
        int count = read(current_device.fd,&c,1);

        //kill if other side closed connection
        if(count == 0)
        {
            std::cout << "[LOCAL] Other side closed" << std::endl;
            break;
        }

        //see if there has been an error
        if(count == -1)
        {
            //kill the entire thing
            std::cout << "[LOCAL] Error, killing connection" << std::endl;
            break;
        }
        

        //see if its ended
        if(c == '\n')
        {
            std::string str = buffer.str();
            handleLocalMessage(str,current_device.fd);

            //clear the buffer for next use
            buffer.str(std::string());
        }else{
            buffer.write(&c,1);
        }
    }
}

//Send a message to the given socket
void sendmessage(int fd,char* raw)
{
    if(current_device.encrypted)
    {
        char* buffer = encrypt(raw,current_device.key,current_device.iv);
        if(buffer == nullptr)
        {
            return;
        }

        write(fd,buffer,strlen(buffer));
        delete buffer;
    }else{
        write(fd,raw,strlen(raw));
    }

    write(fd,"\n",1);
}

void handleLocalMessage(std::string str,int fd)
{
    try{
        json j;
        
        if(current_device.encrypted)
        {
            char* data = decrypt((char*)str.c_str(),current_device.key,current_device.iv);
            if(data == nullptr)
            {
                return;
            }

            j = json::parse(data);
            delete data;
        }else{
            j = json::parse(str.c_str());
        }
        
        if(j["type"].is_null())
        {
            return;
        }

        std::string type = j["type"].get<std::string>();
        if(type == "action")
        {
            //TODO: enable device
        }else if(type == "ack")
        {
            current_device.encrypted = true;
            std::cout << "encrypted enabled" << std::endl;
        }

    }catch(nlohmann::detail::parse_error e)
    {

    }
}