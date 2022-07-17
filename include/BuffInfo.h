//
// Created by Administrator on 2022/7/17.
//

#ifndef TCPSERVER_BUFFINFO_H
#define TCPSERVER_BUFFINFO_H

#include "CommHead.h"
#include "CommBuffer.h"
#include "CommFunction.h"

namespace SelfServer
{
    class BuffInfo final: public CommBuffer
    {
    private:
        std::mutex m_mutex;
    public:
        BuffInfo();
        ~BuffInfo();

        int addData(const char *pval, const int len) override;

        int sendData(SocketFd fd);
    };
}


#endif //TCPSERVER_BUFFINFO_H
