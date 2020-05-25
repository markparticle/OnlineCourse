/*
 * @Author       : mark
 * @Date         : 2020-05-24
 * @copyleft GPL 2.0
 */

#include "server.h"
#include "utils.h"



Server::Server()
 {
     serverAddr_.sin_family = AF_INET;
     serverAddr_.sin_port = htons(SERVER_PORT);
     serverAddr_.sin_addr.s_addr = INADDR_ANY;
     socketFd_ = 0;
     Init();
     Start();
 }

 void Server::Init()
 {
     int ret;
     log("Start Server...");
     socketFd_ = socket(AF_INET, SOCK_STREAM, 0);
     if (socketFd_ < 0)
     {
         perror("Create socket error!...");
         exit(-1);
     }

     ret = bind(socketFd_, (struct sockaddr*)&serverAddr_, sizeof(serverAddr_));
     if (ret < 0)
     {
         perror("Bind socket error!...");
         exit(-1);
     }

     ret = listen(socketFd_, 6);
     if (ret < 0)
     {
         perror("Listen socket error!...");
         exit(-1);
     }
     log("Start to listen: %d", SERVER_PORT);
 }

 void Server::Close()
 {
     closesocket(socketFd_);
     log("Server Closed!...");
 }


 void Server::Start()
 {
     while (true) {
         log("accept \n");
         //struct sockaddr_in clientAddress;
         //socklen_t len = sizeof(clientAddress);
         //int connection = accept(socketFd_, (struct sockaddr*)&clientAddress, &len);
     }
     Close();
 }

