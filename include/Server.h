//
// Created by Administrator on 2022/6/26.
//

#ifndef MYSERVER_SERVER_H
#define MYSERVER_SERVER_H

#include "ClientInfo.h"
#include "CommSingle.h"

namespace SelfServer{
    using ClientPtr=std::shared_ptr<ClientInfo>;
    class Server final{
    public:
        ~Server();
        void runStart();
    private:
        void runThread();
        void closeClient(ClientPtr);

        int initServer();
    private:
        SocketFd m_fd;
        SocketFd m_maxfd;
        fd_set m_socSet;
        int m_port;
        std::unordered_map<int, ClientPtr> m_client;
        bool m_isSocket; /*用来判断服务器是否成功初始化 socket bind listen*/

    CREAT_SINGLE(Server)
    };
}

#endif //MYSERVER_SERVER_H
