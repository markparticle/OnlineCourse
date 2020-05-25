/*
 * @Author       : mark
 * @Date         : 2020-05-21
 * @copyleft GPL 2.0
 */

#ifndef SERVER_H
#define SERVER_H
#include "socket.h"

class Server {
public:
    Server();

    ~Server();

    void Init();

    void Start();

    void Close();

private:
    int socketFd_;

    struct sockaddr_in serverAddr_;
};


#endif //SERVER_H