//
// Created by Administrator on 2022/6/26.
//
#include <fstream>
#include <sstream>

#include "Server.h"

using namespace SelfServer;

#define SERINFOFILE "../config/ServerInfo.txt"

#define READ_INT_NUM(read_file, read_cfg, dtr) \
    {                                          \
        std::ostringstream _sin;\
        std::string _str;                      \
        _sin << read_file.rdbuf();    \
        _str = _sin.str(); \
        auto found_start = _str.find_first_of(read_cfg); \
        if(std::string::npos != found_start) \
        { \
            auto found_end = _str.find(']', found_start); \
            if(std::string::npos != found_end) \
            { \
                std::string tem = _str.substr(0, found_end); \
                std::string::size_type tem_start = tem.find_first_of(read_cfg); \
                std::stringstream sst; \
                sst << tem.substr(tem_start+sizeof(read_cfg)-1); \
                sst >> dtr; \
            }                                      \
        }\
    }


/*调用私有变量初始化--参数的设置需要读文件, 构造函数不能有入参*/
Server::Server() : m_isSocket(false),m_fd(-1), m_port(80)
{
    ClientInfo::s_cliCount = 0;
    if(SELSOCK_OK != initServer()){
        exit(-1);
    }
}

Server::~Server() {}

/*做成一个线程*/
void Server::runStart() {
}

/*读取config里面的配置文件 ServerInfo.txt*/
int Server::initServer()
{
    std::ifstream read_file;
    read_file.open(SERINFOFILE, std::ios::in);
    if(read_file.is_open())
    {
        /*1.读出配置文件中的内容, 用于进程参数的配置*/
        READ_INT_NUM(read_file, "port:", this->m_port)
        std::cout << this->m_port << std::endl;
        /*2.创建socket套接字*/
        if(1024 > this->m_port || 65535 < this->m_port)
        {
            std::cout << "port is error" << std::endl;
            return SELSOCK_ERROR;
        }
        if(!m_isSocket)
        {
            this->m_fd = socket(AF_INET, SOCK_STREAM, 0);
            if(0 > this->m_fd)
            {
                return SELSOCK_ERROR;
            }
            int optval = 1;
            int ret = 0;
            if(SELSOCK_ERROR == (ret = setsockopt(this->m_fd, SOL_SOCKET, SO_REUSEADDR, (const void*)&optval, (socklen_t)sizeof(int))))
            {
                std::cout << "[setsockopt] failed! error: " << strerror(errno) << std::endl;
                return SELSOCK_ERROR;
            }
            else
            {
                std::cout << "[setsockopt] result: " << ret << std::endl;
            }

            /*3.bind*/
            struct sockaddr_in sockaddr;
            sockaddr.sin_family = AF_INET;
            sockaddr.sin_port = htons(this->m_port);
            //sockaddr.sin_addr.s_addr = INADDR_ANY;
            inet_aton("0.0.0.0",&sockaddr.sin_addr);
            if(SELSOCK_ERROR == (bind(this->m_fd, (struct sockaddr*)&sockaddr,sizeof(struct sockaddr_in))))
            {
                std::cout << "[bind] failed! error: " << strerror(errno) << std::endl;
                return SELSOCK_ERROR;
            }

            /*4.listen*/
            if(SELSOCK_ERROR == listen(this->m_fd, 12))
            {
                std::cout << "[listen] failed! error: " << strerror(errno) << std::endl;
                return SELSOCK_ERROR;
            }
            FD_ZERO(&this->m_socSet);
            FD_SET(this->m_fd, &this->m_socSet);
            this->m_maxfd = this->m_fd;

            /*set non blocking*/
            {
                int flags,s;
                flags = fcntl(this->m_fd, F_GETFL, 0);
                if(-1 == flags)
                {
                    std::cout << "[fcntl] failed, error:" << strerror(errno) << std::endl;
                    return SELSOCK_ERROR;
                }
                long arg = flags | O_NONBLOCK; // flags & ~O_NONBLOCK
                s = fcntl(this->m_fd, F_SETFL, arg);
                if(-1 == s)
                {
                    std::cout << "[fcntl] failed, error:" << strerror(errno) << std::endl;
                    return SELSOCK_ERROR;
                }
            }

            this->m_isSocket = true;
        }
    }
    else
    {
        std::cout << "file open failed" << std::endl;
        return SELSOCK_ERROR;
    }
    return SELSOCK_OK;
}