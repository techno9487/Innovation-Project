#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <thread>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <errno.h>
#include "link.hpp"
#include <vector>
#include "json.hh"
#include <unistd.h>

/*
Author: Toby Dunn

This program facilitates communication bettween the web process
and the smart devices in the home.    
*/

//Function containg the thread for accepting external sockets
void externalSocketThread();

//handles the singular local connection
void handleLocalConnection(int fd);

//handles a local message
int handleLocalMessage(std::string &stream,int fd);

int main()
{
    int localSock;
    localSock = socket(AF_UNIX,SOCK_STREAM,0);

    std::thread externalThread(externalSocketThread);
    
    sockaddr_un socketDescriptor;
    socketDescriptor.sun_family = AF_UNIX;
    strcpy(socketDescriptor.sun_path,"/tmp/project.sock");

    if(bind(localSock,(sockaddr*)&socketDescriptor,sizeof(socketDescriptor)) != 0){
        std::cout << "unable to bind unix socket:" << errno << std::endl;
        return 1;
    }

    listen(localSock,10);

    while(true)
    {
        sockaddr_un socket_info;
        socklen_t size = sizeof(socket_info);

        int fd = accept(localSock,(sockaddr*)&socket_info,&size);
        if(fd != -1)
        {
            std::cout << "[LOCAL] New connection" << std::endl;
            handleLocalConnection(fd);
        }else{
            std::cout << "[LOCAL] error acceptign connection: " << errno << std::endl;
        }

    }

    remove("/tmp/project.sock");

    externalThread.join();

    return 0;
}

//Contains all currently executign external threads
std::vector<DeviceThread*> externalThreads;

void externalSocketThread()
{
    int externalSock = socket(AF_INET,SOCK_STREAM,0);

    sockaddr_in address = {};
    address.sin_family = AF_INET;
    address.sin_port = htons(1025);
    address.sin_addr.s_addr = inet_addr("0.0.0.0");

    if(bind(externalSock,(sockaddr*)&address,sizeof(address)) < 0)
    {
        std::cout << "Can't bond to socket" << std::endl;
        return;
    }

    listen(externalSock,10);

    bool isRunning = true;

    while(isRunning)
    {
        sockaddr_in client_addr = {};
        socklen_t size = sizeof(client_addr);
        int fd = accept(externalSock,(sockaddr*)&client_addr,&size);

        //check if actual connection
        if(fd != -1)
        {
            char ip_buffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(client_addr.sin_addr), ip_buffer, INET_ADDRSTRLEN);
            printf("Connection from %d %s\n",fd,ip_buffer);

            
            DeviceThread* thread = new DeviceThread;
            DeviceLink* link = new DeviceLink(fd,&client_addr,size,thread);
            thread->link = link;
            thread->thread = std::thread(&DeviceLink::run,link);

            externalThreads.push_back(thread);
        }else if(errno != EBADF)
        {
            printf("Error: %d\n",errno);
        }
    }

    std::vector<DeviceThread*>::iterator it;
    for(it = externalThreads.begin(); it<externalThreads.end();it++)
    {
        DeviceThread* thread = (*it);
        thread->thread.join();
        delete thread->link;
        delete thread;
    }
}

void handleLocalConnection(int fd)
{
    std::stringstream buffer;
    char c;
    while(true)
    {

        int count = read(fd,&c,1);

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
            handleLocalMessage(str,fd);

            //clear the buffer for next use
            buffer.str(std::string());
        }else{
            buffer.write(&c,1);
        }
    }
}

int handleLocalMessage(std::string &str,int fd)
{
    using json = nlohmann::json;

    try{
        json j = json::parse(str.c_str());
        
        //check the required fields exist
        if(j["device_id"].is_null())
        {
            return 1;
        }

        if(j["content"].is_null())
        {
            return 1;
        }

        //pull out the data we need
        int device_id = j["device_id"].get<int>();
        std::string content = j["content"].get<std::string>();

        //now to see if the requested device is connected
        std::vector<DeviceThread*>::iterator it;
        bool found = false;
        for(it = externalThreads.begin(); it != externalThreads.end(); it++)
        {
            DeviceThread* thread = (*it);
            if(thread->iDeviceID == device_id)
            {
                //found it
                found = true;
                thread->link->sendmessage((char*)content.c_str());
                std::cout << "Message sent to local device" << std::endl;
            }
        }

        //generate a response to the webserver
        json resp;
        
        if(found)
        {
            resp["success"] = true;
        }else{
            resp["success"] = false;
            std::cout << "Unable to find device" << std::endl;
        }

    }catch(nlohmann::detail::parse_error e)
    {

    }

    return 0;
}