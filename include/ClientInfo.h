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

    public:
        static std::atomic<int> s_cliCount;
    private:
        bool m_isSock;
        SocketFd m_fd;
        std::shared_ptr<struct sockaddr_in> p_addr;
        struct recvData
        {
            recvData(int len):m_maxsize(len), m_cursize(0),pdata(new char[len]){}
            recvData(int len, recvData& val):m_maxsize(len), m_cursize(val.m_cursize),pdata(new char[len])
            {
                memmove(pdata.get(), val.pdata.get(), val.m_cursize);
            }
            ~recvData(){}

            recvData& operator=(const recvData & val)
            {
                this->pdata = val.pdata;
                this->m_maxsize = val.m_maxsize;
                this->m_cursize = val.m_cursize;
                return *this;
            }

            std::shared_ptr<char> pdata;
            int m_maxsize;
            int m_cursize;
        };
        std::vector<recvData> m_recvBuf; /* 读到的客户端数据 */
        BuffPtr p_sendBuf; /* 发送数据存放的位置 */
    };
}
#endif //MYSERVER_CLIENTINFO_H
