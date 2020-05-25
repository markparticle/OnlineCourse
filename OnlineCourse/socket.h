/*
 * @Author       : mark
 * @Date         : 2020-05-24
 * @copyleft GPL 2.0
 */

#ifndef SOCKET_H
#define SOCKET_H

#include <tchar.h>
#include <winsock2.h>
#include <windows.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")


const int SERVER_PORT = 8965;

const char SERVER_IP[] = "127.0.0.1";

const int EPOLL_SIZE = 5000;

const int BUFF_SIZE = 0xFFFF;

const int TIMEOUT = -1;



#endif //SOCKET_H