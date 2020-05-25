/*
 * @Author       : mark
 * @Date         : 2020-05-24
 * @copyleft GPL 2.0
 */

#include "utils.h"
#include "client.h"


Client::Client()
{
    serverAddr_.sin_family = AF_INET;
    serverAddr_.sin_port = htons(SERVER_PORT);
    serverAddr_.sin_addr.s_addr = inet_addr(SERVER_IP);
    socketFd_ = 0;
    Start();
}

void Client::Start()
{
    connect(socketFd_, (struct sockaddr*)&serverAddr_, sizeof(serverAddr_));
    while (true) {
        
    }
    Close();
}


void Client::Close()
{
    closesocket(socketFd_);
    log("Server Closed!...");
}
