//
// Created by Administrator on 2022/6/26.
//

#ifndef MYSERVER_CLIENTINFO_H
#define MYSERVER_CLIENTINFO_H
#include <atomic>

#include "CommHead.h"

namespace SelfServer{
    class ClientInfo{
    public:
        ClientInfo();
        ~ClientInfo();
        int init(int);
        SocketFd getFd() const;
        bool getisSock() const;

    public:
        static std::atomic<int> s_cliCount;
    private:
        bool m_isSock;
        SocketFd m_fd;
        std::vector<char> m_readBuf; /* 读到的客户端数据 */
    };
}
#endif //MYSERVER_CLIENTINFO_H
