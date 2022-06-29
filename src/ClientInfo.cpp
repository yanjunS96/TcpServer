//
// Created by Administrator on 2022/6/26.
//
#include "ClientInfo.h"

using namespace SelfServer;

std::atomic<int> ClientInfo::s_cliCount;

ClientInfo::ClientInfo() {}

ClientInfo::~ClientInfo() {}

SocketFd ClientInfo::getFd() const {
    return this->m_fd;
}

bool ClientInfo::getisSock() const {
    return this->m_isSock;
}

