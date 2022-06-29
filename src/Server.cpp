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
                std::string tem = _str.substr(0, found_end - 1); \
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
    initServer();
}

Server::~Server() {}

/*做成一个线程*/
void Server::runStart() {
}

/*读取config里面的配置文件 ServerInfo.txt*/
void Server::initServer()
{
    std::ifstream read_file;
    read_file.open(SERINFOFILE, std::ios::in);
    if(read_file.is_open())
    {
        /*1.读出配置文件中的内容, 用于进程参数的配置*/
        READ_INT_NUM(read_file, "port:", this->m_port)
        std::cout << this->m_port << std::endl;
        /*2.创建socket套接字*/
        /*3.bind*/
        /*4.listen*/
        this->m_isSocket = true;
    }
    else
    {
        std::cout << "file open failed" << std::endl;
        return;
    }
}