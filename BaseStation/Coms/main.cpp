#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <thread>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <errno.h>
#include "link.hpp"

/*
Author: Toby Dunn

This program facilitates communication bettween the web process
and the smart devices in the home.    
*/

void externalSocketThread();

int main()
{
    int localSock;
    localSock = socket(AF_UNIX,SOCK_STREAM,0);

    std::thread externalThread(externalSocketThread);
    
    sockaddr_un socketDescriptor;
    socketDescriptor.sun_family = AF_UNIX;
    strcpy(socketDescriptor.sun_path,"/tmp/project.sock");

    bind(localSock,(sockaddr*)&socketDescriptor,sizeof(socketDescriptor));

    //TODO: Receive data requests from the web server

    remove("/tmp/project.sock");

    externalThread.join();

    return 0;
}

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
        //TODO: Accept external requests from the devices themselves
        //then spawn a new thread to handle that connection

        sockaddr_in client_addr = {};
        socklen_t size = sizeof(client_addr);
        int fd = accept(externalSock,(sockaddr*)&client_addr,&size);

        //check if actual connection
        if(fd != -1)
        {
            char ip_buffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(client_addr.sin_addr), ip_buffer, INET_ADDRSTRLEN);
            printf("Connection from %d %s\n",fd,ip_buffer);

            DeviceLink* link = new DeviceLink(fd,&client_addr,size);
            std::thread link_thread(&DeviceLink::run,link);
            link_thread.join();
            
            //TODO: possibly handle threads
        }else if(errno != EBADF)
        {
            printf("Error: %d\n",errno);
        }
    }
}