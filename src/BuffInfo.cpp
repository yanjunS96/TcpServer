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
    cycTryLock(this->m_mutex);
    ret = socket_send(fd, this->p_data.get(), this->m_curlen);
    if(ret != this->m_curlen) // 发送失败了或者是发到一半断网了
    {
        //do nothing
    }
    memset(this->p_data.get(), 0, this->m_curlen);
    this->m_curlen = 0;
    this->m_mutex.unlock();
    return ret;
}
