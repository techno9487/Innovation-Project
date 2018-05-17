#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <thread>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

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
    address.sin_addr.s_addr = inet_addr("192.168.0.1");

    bind(externalSock,(sockaddr*)&address,sizeof(address));

    while(true)
    {
        //TODO: Accept external requests from the devices themselves
        //then spawn a new thread to handle that connection

        sockaddr_in client_addr = {};
        socklen_t size = sizeof(client_addr);
        int fd = accept(fd,(sockaddr*)&client_addr,&size);
    }
}