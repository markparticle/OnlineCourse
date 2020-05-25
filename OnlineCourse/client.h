/*
 * @Author       : mark
 * @Date         : 2020-05-25
 * @copyleft GPL 2.0
 */

#ifndef CLIENT_H
#define CLIENT_H
#include "socket.h"

class Client {
public:
    Client();

    void Start();

    void Close();

private:
    int socketFd_;
    struct sockaddr_in serverAddr_;
};



#endif //CLIENT_H