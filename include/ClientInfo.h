//
// Created by Administrator on 2022/6/26.
//

#ifndef MYSERVER_CLIENTINFO_H
#define MYSERVER_CLIENTINFO_H
#include <atomic>

#include "BuffInfo.h"
#include "CommFunction.h"

namespace SelfServer{
#define MAX_FD 65535
#define DEF_RECV_LEN 1024

    using BuffPtr = std::shared_ptr<BuffInfo>;

    class ClientInfo{
    public:
        ClientInfo();
        ~ClientInfo();

        int init(int, const struct sockaddr_in&);

        int readData();
        void connClose();

        SocketFd getFd() const;
        bool getisSock() const;

    private:
        //解析接受数据的线程
        void parseRecvBuff();

    public:
        static std::atomic<int> s_cliCount;
    private:
        bool m_isSock;
        SocketFd m_fd;
        std::shared_ptr<struct sockaddr_in> p_addr;
        std::vector<BuffData> m_recvBuf; /* 读到的客户端数据 */
        BuffPtr p_sendBuf; /* 发送数据存放的位置 */
    };
}
#endif //MYSERVER_CLIENTINFO_H
