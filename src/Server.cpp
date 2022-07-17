//
// Created by Administrator on 2022/6/26.
//
#include <fstream>
#include <sstream>
#include <functional>

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
Server::Server()
    : m_isSocket(false),
    m_fd(-1),
    m_maxfd(-1),
    m_port(80)
{
    ClientInfo::s_cliCount = 0;
    if(SOCKET_OK == initServer())
    {
        m_isSocket = true;
    }
}

Server::~Server() {}

void Server::runStart() {
    if(!this->m_isSocket)
    {
        if(SOCKET_OK == initServer()){this->m_isSocket = true;}
        else
        {
            std::cout << "##error## Server init failed! exit process" << std::endl;
            exit(-1);
        }
    }
    //接受客户端链接, 并循环读取客户端发送过来的数据
    /*std::thread(
            std::bind(&Server::runThread, this)
            ).detach();*/
    runThread(); //主线程阻塞
    return;
}

void Server::runThread()
{
    int result = -1;
    while(this->m_isSocket)
    {
        fd_set readSet = this->m_socSet;
        result = select(this->m_maxfd + 1, &readSet, NULL, NULL, NULL);
        if (SOCKET_ERROR == result)
        {
            std::cout << "select() returned with error:" << strerror(errno) << std::endl;
            continue;
        }
        //如果存在新的客户端接入
        if (FD_ISSET(this->m_fd, &readSet))
        {
            struct sockaddr_in addr;
            socklen_t len = sizeof(addr);

            /* accept接收 */
            SocketFd fd = accept(this->m_fd, (struct sockaddr *) &addr, &len);
            if (fd <= 0) {
                std::cout << "accept failed!errno: " << strerror(errno) << ", fd: " << fd /*<< "[" << __FILE__ << " "
                                  << __LINE__ << " " << __FUNCTION__ << "]"*/ << std::endl;
                continue;
            }
            else if (ClientInfo::s_cliCount >= MAX_FD)
            {
                //commSend(fd, "Server busy!");
                std::cout << "Sorry, Clients is full!" << std::endl;
                continue;
            }
            if(nullptr == this->m_client[fd])
            {
                this->m_client[fd] = ClientPtr(new ClientInfo());
            }
            this->m_client[fd]->init(fd, addr);
            std::cout << result << ", have a new request, now client_num: " << ClientInfo::s_cliCount << std::endl;
            FD_SET(fd, &m_socSet);  /*将套接字加入到监听的 select 套接字集合*/
            m_maxfd = (m_maxfd>fd)?m_maxfd:fd;
            /*set non blocking*/
            if(SOCKET_ERROR == setNoBlocking(fd)){continue;}

            /* 如果没有更多的就绪文件描述符继续回到上面select阻塞监听,负责处理未处理完的就绪文件描述符 */
            if (--result == 0)
                continue;
        }

        /* //遍历所有客户端 接收客户端数据 并将数据放在客户端 recvBUf */
        auto client_p = this->m_client.begin();
        /*for(int i = 0; i < CommServerInfo::s_usedCount;client_p++, i++)*/
        for(; client_p != this->m_client.end(); client_p++)
        {
            int tem_fd = client_p->first;
            /* 如果这个是否需要读 */
            if((nullptr == client_p->second) || !client_p->second->getisSock()){continue;}

            if(FD_ISSET(tem_fd, &readSet))
            {
                int rec_len = client_p->second->readData();
                if(0 == rec_len || -1 == rec_len)
                {
                    //没收到数据, 关闭客户端
                    closeClient(client_p->second);
                }
                if (--result == 0)
                    break;
            }
        }
        /*1ms循环一次*/
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return;
}

void Server::closeClient(ClientPtr p_client) {
    if(nullptr != p_client)
    {
        FD_CLR(p_client->getFd(), &this->m_socSet);
        p_client->connClose();
    }
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
            return SOCKET_ERROR;
        }
        if(!m_isSocket)
        {
            this->m_fd = socket(AF_INET, SOCK_STREAM, 0);
            if(0 > this->m_fd)
            {
                return SOCKET_ERROR;
            }
            int optval = 1;
            int ret = 0;
            if(SOCKET_ERROR == (ret = setsockopt(this->m_fd, SOL_SOCKET, SO_REUSEADDR, (const void*)&optval, (socklen_t)sizeof(int))))
            {
                std::cout << "[setsockopt] failed! error: " << strerror(errno) << std::endl;
                return SOCKET_ERROR;
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
            if(SOCKET_ERROR == (bind(this->m_fd, (struct sockaddr*)&sockaddr,sizeof(struct sockaddr_in))))
            {
                std::cout << "[bind] failed! error: " << strerror(errno) << std::endl;
                return SOCKET_ERROR;
            }

            /*4.listen*/
            if(SOCKET_ERROR == listen(this->m_fd, 12))
            {
                std::cout << "[listen] failed! error: " << strerror(errno) << std::endl;
                return SOCKET_ERROR;
            }
            FD_ZERO(&this->m_socSet);
            FD_SET(this->m_fd, &this->m_socSet);
            this->m_maxfd = this->m_fd;

            /*set non blocking*/
            if(SOCKET_ERROR == setNoBlocking(this->m_fd))
            {
                return SOCKET_ERROR;
            }

            this->m_isSocket = true;
        }
    }
    else
    {
        std::cout << "file open failed" << std::endl;
        return SOCKET_ERROR;
    }
    return SOCKET_OK;
}