//
// Created by Administrator on 2022/7/17.
//

#ifndef TCPSERVER_COMMFUNCTION_H
#define TCPSERVER_COMMFUNCTION_H
/* 网络编程相关头文件 */
#include <sys/types.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>

#include "CommHead.h"


#define SOCKET_OK 0
#define SOCKET_ERROR -1
#define SOCKET_TIMEOUT -2
#define SOCKET_CLOSE -3
typedef int SocketFd;

int socket_close(SocketFd);
int socket_send(SocketFd ,const char * , int );
int socket_send(SocketFd, const std::string&);
int socket_send(SocketFd, const std::string&&);
int socket_recv(SocketFd , char *, int);
int setNoBlocking(SocketFd, bool = true);

void cycTryLock(std::mutex &);

#endif //TCPSERVER_COMMFUNCTION_H
