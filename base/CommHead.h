//
// Created by Administrator on 2022/6/26.
//

#ifndef MYSERVER_COMMHEAD_H
#define MYSERVER_COMMHEAD_H
#include <iostream>
#include <vector>
#include <string>
/*
#include <list>
#include <forward_list>
#include <deque>
#include <queue>
*/
#include <map>
#include <unordered_map>
/*
#include <set>
#include <unordered_set>
*/

#include <memory>
#include <mutex>

#include <error.h>
#include <string.h>

/* 网络编程相关头文件 */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <fcntl.h>

typedef int SocketFd;
#define SELSOCK_OK 0
#define SELSOCK_ERROR -1
#define SELSOCK_TIMEOUT -2

#endif //MYSERVER_COMMHEAD_H
