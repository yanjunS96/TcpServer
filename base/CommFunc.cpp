//
// Created by Administrator on 2022/7/17.
//
#include "CommFunction.h"
/* 网络编程相关头文件 */
#include <unistd.h> //close()

int socket_close(SocketFd fd)
{
    if(fd < 0){return SOCKET_ERROR;}
    shutdown(fd, SOCKET_ERROR);
    if(close(fd) < 0)
    {
        std::cout << "close error: " << strerror(errno) << std::endl;
    }
    fd = -1;
    return SOCKET_OK;
}

int socket_send(SocketFd fd,const char * buff, int len)
{
    if ((fd < 0) || (nullptr == buff) || (0 >= len))
    {
        std::cout << "[socket_send] input paramater is error, fd: " << fd << std::endl;
        return SOCKET_ERROR;
    }
    int need_len = len; /* 剩余需要发送的长度 */
    int _sended = 0; /* 每次循环内发送的长度 */
    char const* p_val = buff;
    while(need_len > 0) /*还有数据需要发送*/
    {
        //if ((_sended = send(fd, static_cast<const void*>(p_val), static_cast<size_t>(need_len), 0)) == SOCKET_ERROR)
        if ((_sended = send(fd, static_cast<const void*>(p_val), static_cast<size_t>(need_len), MSG_NOSIGNAL)) == SOCKET_ERROR) /*禁止send()函数向系统发送异常消息*/
        {
            if(EAGAIN == errno) /*发送缓冲区满了*/
            {
                //std::cout << "####send buff full, len: " << _sended << std::endl;
                continue;
            }
            else if(EACCES == errno)
            {
                std::cout << "##error##The calling process doesn't have the appropriate privileges." << std::endl;
                break;
            }
            else if(EBADF == errno)
            {
                std::cout << "##error##An invalid descriptor was specified." << std::endl;
                socket_close(fd);
                break;
            }
            else if(ECONNRESET == errno)
            {
                std::cout << "##error##A connection was forcibly closed by a peer." << std::endl;
                socket_close(fd);
                break;
            }
            else if(EDESTADDRREQ == errno)
            {
                std::cout << "##error##The socket is not connection-mode and no peer address is set." << std::endl;
                break;
            }
            else if(EFAULT == errno)
            {
                std::cout << "##error##An invalid user space address was specified for a parameter." << std::endl;
                break;
            }
            else if(EINTR == errno)
            {
                std::cout << "##error##A signal interrupted send() before any data was transmitted." << std::endl;
                break;
            }
            else if(EIO == errno)
            {
                std::cout << "##error##An I/O error occurred while reading from or writing to the filesystem." << std::endl;
                break;
            }
            else if(EMSGSIZE == errno)
            {
                std::cout << "##error##The socket requires that the message be sent atomically, but the size of the message made this impossible." << std::endl;
                break;
            }
            else if(ENETDOWN == errno)
            {
                std::cout << "##error##The local network interface used to reach the destination is down." << std::endl;
                break;
            }
            else if(ENETUNREACH == errno)
            {
                std::cout << "##error##No route to the network is present." << std::endl;
                break;
            }
            else if(ENOBUFS == errno)
            {
                std::cout << "##error##The system couldn't allocate an internal buffer. The operation may succeed when buffers become available." << std::endl;
                break;
            }
            else if(ENOTCONN == errno)
            {
                std::cout << "##error##The socket isn't connected or otherwise has not had the peer pre-specified." << std::endl;
                break;
            }
            else if(ENOTSOCK == errno)
            {
                std::cout << "##error##The argument s isn't a socket." << std::endl;
                break;
            }
            else if(EOPNOTSUPP == errno)
            {
                std::cout << "##error##The socket argument is associated with a socket that doesn't support one or more of the values set in flags." << std::endl;
                break;
            }
            else if(EOVERFLOW == errno)
            {
                std::cout << "##error##An attempt was made to send an amount of data that when added to the size of the socket send message structure exceeds the allowable limit." << std::endl;
                break;
            }
            else if(EPIPE == errno)
            {
                std::cout << "##error##The socket is shut down for writing, or the socket is connection-mode and is no longer connected. In the latter case, and if the socket is of type SOCK_STREAM, a SIGPIPE signal is generated to the calling thread." << std::endl;
                break;
            }
            else if(EWOULDBLOCK == errno)
            {
                std::cout << "##error##The socket is marked nonblocking and the requested operation would block." << std::endl;
                break;
            }
            else /*发送失败了*/
            {
                std::cout << "##error##send failed! fd:" << fd << ", errno:" << strerror(errno) << ", len=" << _sended << std::endl;
                break;
            }
        }
        else /*数据发送成功了*/
        {
            p_val += _sended;
            need_len -= _sended;
        }
    }
    return (len - need_len);
}

int socket_send(SocketFd fd, const std::string & str)
{
    if((0 >= str.size()) || (fd < 0))
    {
        return SOCKET_ERROR;
    }
    return socket_send(fd, (const char *)str.data(), (int)str.size());
}

int socket_send(SocketFd fd, const std::string && str)
{
    if((0 >= str.size()) || (fd < 0))
    {
        return SOCKET_ERROR;
    }
    std::string tem(str);
    return socket_send(fd, (const char *)tem.c_str(), (int)tem.size());
}

int socket_recv(SocketFd fd, char * buff, int len)
{
    if ((fd < 0) || (nullptr == buff) || (0 >= len))
    {
        std::cout << "[socket_recv] paramater is error, fd: " << fd << std::endl;
        return SOCKET_ERROR;
    }
    int ret = 0;
    ret = recv(fd, static_cast<void*>(buff), static_cast<size_t>(len), 0);
    if (ret == 0)
    {
        /* 收到的数据长度为0, 对端链路关闭了 */
        std::cout << "recv failed! fd:" << fd << ", errno:" << strerror(errno) << ", len=" << ret << std::endl;
        ret = SOCKET_CLOSE;
    }
    else if(SOCKET_ERROR == ret)
    {
        if(ECONNRESET == errno)
        {
            std::cout << "ECONNRESET failed! fd:" << fd << ", errno:" << strerror(errno) << ", len=" << ret << std::endl;
            ret = SOCKET_CLOSE;
        }
        else if(EBADF == errno)
        {
            std::cout << "Invalid descriptor s. fd:" << fd << ", errno:" << strerror(errno) << ", len=" << ret << std::endl;
            socket_close(fd);
            ret = SOCKET_ERROR;
        }
        else if(EFAULT == errno)
        {
            std::cout << "The receive buffer is outside the process's address space. fd:" << fd << ", errno:" << strerror(errno) << ", len=" << ret << std::endl;
            ret = SOCKET_CLOSE;
        }
        else if(EINTR == errno)
        {
            std::cout << "The receive was interrupted by delivery of a signal before any data was available. fd:" << fd << ", errno:" << strerror(errno) << ", len=" << ret << std::endl;
            ret = SOCKET_CLOSE;
        }
        else if(ENOTCONN == errno)
        {
            std::cout << "The socket is associated with a connection-oriented protocol and hasn't been connected; see connect() and accept(). fd:" << fd << ", errno:" << strerror(errno) << ", len=" << ret << std::endl;
            ret = SOCKET_CLOSE;
        }
        else if(EOVERFLOW == errno)
        {
            std::cout << "An attempt was made to receive an amount of data that exceeds the allowable limit.! fd:" << fd << ", errno:" << strerror(errno) << ", len=" << ret << std::endl;
            ret = SOCKET_CLOSE;
        }
        else if(EWOULDBLOCK == errno)
        {
            std::cout << "Either the socket is marked nonblocking and the receive operation would block, or a receive timeout had been set and the timeout expired before data was received. fd:" << fd << ", errno:" << strerror(errno) << ", len=" << ret << std::endl;
            ret = SOCKET_CLOSE;
        }
        else
        {
            std::cout << "recv failed! fd:" << fd << ", errno:" << strerror(errno) << ", len=" << ret << std::endl;
            ret = SOCKET_ERROR;
        }
    }
    return ret;
}

int setNoBlocking(SocketFd fd, bool isnonblock)
{
    int flags, s;
    flags = fcntl(fd, F_GETFL, 0);
    if (-1 == flags) {
        std::cout << "[setNoBlocking]fcntl F_GETFL failed, error: " << strerror(errno) << std::endl;
        return SOCKET_ERROR;
    }

    long arg = (isnonblock) ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
    s = fcntl(fd, F_SETFL, arg);
    if (-1 == s) {
        std::cout << "[setNoBlocking]fcntl F_SETFL failed, error: " << strerror(errno) << std::endl;
        return SOCKET_ERROR;
    }
    return SOCKET_OK;
}

void cycTryLock(std::mutex & val)
{
    if(!val.try_lock())
    {
        usleep(5);
        cycTryLock(val);
    }
}
