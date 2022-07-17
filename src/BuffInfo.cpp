//
// Created by Administrator on 2022/7/17.
//

#include "BuffInfo.h"

using namespace SelfServer;

BuffInfo::BuffInfo() : CommBuffer() {}

BuffInfo::~BuffInfo() noexcept {}

int BuffInfo::addData(const char *pval, const int len)
{
    int ret = 0;
    cycTryLock(this->m_mutex);
    ret = this->addVal(pval, len);
    this->m_mutex.unlock();
    return ret;
}

int BuffInfo::sendData(SocketFd fd)
{
    int ret = 0;
    if((nullptr != this->m_data.pdata) && (0 < this->m_data.m_cursize))
    {
        cycTryLock(this->m_mutex);
        ret = socket_send(fd, this->m_data.pdata.get(), this->m_data.m_cursize);
        if(ret != this->m_data.m_cursize) // 发送失败了或者是发到一半断网了
        {
            //do nothing 不需要做处理 socket_send中已做了
        }
        memset(this->m_data.pdata.get(), 0, this->m_data.m_cursize);
        this->m_data.m_cursize = 0;
        this->m_mutex.unlock();
    }
    return ret;
}
