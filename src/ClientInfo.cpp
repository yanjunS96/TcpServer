//
// Created by Administrator on 2022/6/26.
//
#include "ClientInfo.h"

using namespace SelfServer;

std::atomic<int> ClientInfo::s_cliCount;

ClientInfo::ClientInfo()
: m_isSock(false), m_fd(-1)
 ,p_addr(new struct sockaddr_in)
 ,p_sendBuf(new BuffInfo)
{}

ClientInfo::~ClientInfo() {}

int ClientInfo::init(int fd, const struct sockaddr_in& addr)
{
    if(fd < 0)
    {
        std::cout << "add client sockfd is error!" << std::endl;
        return SOCKET_ERROR;
    }
    ClientInfo::s_cliCount++;
    this->m_fd = fd;
    memmove(this->p_addr.get(), &addr, sizeof(struct sockaddr_in));
    this->m_isSock = true;
    return SOCKET_OK;
}

int ClientInfo::readData() {
    int usr_buf = 0; //表示 buff 里面已经存在的数据长度
    if(this->m_isSock)
    {
        recvData buff(DEF_RECV_LEN);
        int buff_len = DEF_RECV_LEN;
        int recv_len = 0;
        do {
            if((buff_len - usr_buf) < DEF_RECV_LEN)
            {
                int len = 0;
                if(usr_buf > (DEF_RECV_LEN << 3)) //大于 8 倍
                {
                    len = usr_buf;
                }
                else
                {
                    len = DEF_RECV_LEN;
                }
                buff_len = usr_buf + (len << 1);
                recvData tem_buff(buff_len, buff);
                buff = tem_buff;
            }
            recv_len = socket_recv(this->m_fd, buff.pdata.get() + usr_buf, DEF_RECV_LEN);
            if(recv_len < 0) /*数据接收失败*/
            {
                std::cout << "recv error, error message:" << strerror(errno) << std::endl;
                break;
            }
            else /* 接收成功 */
            {
                if(recv_len < DEF_RECV_LEN)/* 接收完了 */
                {
                    usr_buf += recv_len;
                    break;
                }
                else
                {
                    usr_buf += DEF_RECV_LEN;
                }
            }
        } while (true);
        m_recvBuf.push_back(buff);
    }
    return usr_buf;
}

void ClientInfo::connClose() {
    if(this->m_isSock)
    {
        this->m_isSock = false;
        ClientInfo::s_cliCount--;
        this->m_recvBuf.clear();
        std::cout<<"Client fd:"<< this->m_fd <<" quit, userCount:"<<(int)ClientInfo::s_cliCount << std::endl;
        socket_close(this->m_fd);
    }
}

SocketFd ClientInfo::getFd() const {
    return this->m_fd;
}

bool ClientInfo::getisSock() const {
    return this->m_isSock;
}

